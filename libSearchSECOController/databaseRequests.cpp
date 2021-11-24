/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "databaseRequests.h"
#include "networking.h"
#include "print.h"
#include "utils.h"
#include <set>

std::string DatabaseRequests::uploadHashes(std::vector<HashData> &hashes, 
	ProjectMetaData metaData, 
	AuthorData &authorData,
	EnvironmentDTO* env,
	std::string prevCommitTime,
	std::vector<std::string> unchangedFiles)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAllDataFromHashes(
		hashes, 
		dataSize, 
		metaData.getAsHeader(), 
		authorData, 
		prevCommitTime, 
		unchangedFiles);

	print::log("Uploading " + std::to_string(hashes.size()) + " methods to the database", __FILE__, __LINE__);
	return execRequest(DATABASE_UPLOAD_REQUEST, rawData, dataSize, env);
}

std::string DatabaseRequests::findMatches(std::vector<HashData> &hashes, EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getHashDataFromHashes(hashes, dataSize);

	return execRequest(DATABASE_CHECK_REQUEST, rawData, dataSize, env);
}

std::string DatabaseRequests::checkUploadHashes(std::vector<HashData> &hashes,
	ProjectMetaData metaData, 
	AuthorData &authorData,
	EnvironmentDTO* env,
	std::string prevCommitTime,
	std::vector<std::string> unchangedFiles)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAllDataFromHashes(
		hashes, 
		dataSize,
		metaData.getAsHeader(), 
		authorData,
		prevCommitTime,
		unchangedFiles);

	return execRequest(DATABASE_CHECK_UPLOAD_REQUEST, rawData, dataSize, env);
}

std::string DatabaseRequests::getAuthor(const std::map<std::string, int> &authors, 
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAuthorStringToSend(authors, dataSize);

	return execRequest(DATABASE_GET_AUTHORS_BY_ID, rawData, dataSize, env);
}

std::string DatabaseRequests::getProjectData(const std::set<std::pair<std::string, std::string>> &projects,
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getProjectsRequest(projects, dataSize);

	return execRequest(DATABASE_GET_PROJECT_DATA, rawData, dataSize, env);
}

long long DatabaseRequests::getProjectVersion(const std::pair<std::string, std::string>& project,
	EnvironmentDTO* env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getProjectRequest(project, dataSize);

	std::string response = execRequest(DATABASE_GET_MOST_RECENT_VERSION_PROJECT, rawData, dataSize, env);
	
	if (response == "No results found.")
	{
		return 0;
	}
	else
	{
		long long version = 0;
		try
		{
			version = std::stoll(Utils::split(Utils::split(response, ENTRY_DELIMITER)[0], INNER_DELIMITER)[1]);
		}
		catch (const std::exception& e)
		{
			print::warn(
				"Something went wrong while retrieving version of project from database, returning versiontime of 0.",
				__FILE__, 
				__LINE__);
			version = 0;
		}
		return version;
	}
}

std::string DatabaseRequests::getNextJob(EnvironmentDTO* env)
{
	return execRequest(DATABASE_GET_NEXT_JOB, nullptr, 0, env);
}

void DatabaseRequests::updateJob(std::string jobid, std::string &jobTime, EnvironmentDTO *env)
{
	int dataSize = 0;
	const char *rawData = NetworkUtils::getUpdateJobRequest(jobid, jobTime, dataSize);
	jobTime = execRequest(DATABASE_UPDATE_JOB, rawData, dataSize, env);
}

void DatabaseRequests::finishJob(std::string jobid, std::string jobTime, FinishReason code, std::string reason, EnvironmentDTO *env)
{
	if (env->commandString == "start" && errno != HANDLED_ERRNO)
	{
		int dataSize = 0;
		const char *rawData = NetworkUtils::getFinishJobRequest(jobid, jobTime, code, reason, dataSize);
		errno = 0;
		execRequest(DATABASE_FINISH_JOB, rawData, dataSize, env);
		if (errno == 0)
		{
			errno = HANDLED_ERRNO;
		}
	}
}

std::string DatabaseRequests::getIPs(EnvironmentDTO *env)
{
	std::string result = execRequest(DATABASE_GET_IPS, nullptr, 0, env);
	return result;
}

std::string DatabaseRequests::addCrawledJobs(
	const CrawlData& jobs,
	std::string id,
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getUploadCrawlRequest(jobs, id, dataSize);
	return execRequest(DATABASE_CRAWL_DATA, rawData, dataSize, env);
}

std::string DatabaseRequests::execRequest(
	std::string request, 
	const char* rawData, 
	int dataSize, 
	EnvironmentDTO *env)
{
	// First start the connection.
	NetworkHandler* networkHandler = startConnection(env);

	// Then send the header (what request we are doing and how much data we are sending).
	std::string requestType = request + INNER_DELIMITER + env->workerName + INNER_DELIMITER + std::to_string(dataSize) + "\n";
	networkHandler->sendData(requestType);

	// After that we send the data.
	networkHandler->sendData(rawData, dataSize);

	// Then we wait for the output that the API gives us.
	std::string output = networkHandler->receiveData();

	// Close the connection.
	delete networkHandler;

	// Handle the response code.
	std::tuple<bool, std::string> tuple = checkResponseCode(output, env->commandString);
	auto retries = 0;
	
	// Retry if failed.
	while (!std::get<0>(tuple) && retries < REQUEST_RETRIES)
	{
		auto waitFor = (int)pow(2, retries++) * BASE_RETRY_WAIT;
		print::warn(
			"Retrying in " + 
			std::to_string(waitFor / 1000) + 
			" seconds... (" + std::to_string(REQUEST_RETRIES - retries) + 
			" left)"
			, __FILE__, __LINE__);

		std::this_thread::sleep_for(
			std::chrono::milliseconds(waitFor)
			);

		networkHandler = startConnection(env);
		networkHandler->sendData(requestType);
		networkHandler->sendData(rawData, dataSize);
		output = networkHandler->receiveData();
		delete networkHandler;

		// Handle the response code.
		tuple = checkResponseCode(output, env->commandString);
	}

	// Deleting the data we send.
	delete[] rawData;

	if (!std::get<0>(tuple)) 
	{
		print::warn("Ran out of retries. Skipping project...", __FILE__, __LINE__);
	}

	return std::get<1>(tuple);
}

std::tuple<bool, std::string> DatabaseRequests::checkResponseCode(std::string data, std::string command)
{
	std::string statusCode = Utils::split(data, '\n')[0];
	
	std::string info = "";
	if (data.length() >= 4) 
	{
		info = data.substr(4, data.length() - 4);
	}

	if (statusCode == "200") 
	{
		print::debug("Database request successful", __FILE__, __LINE__);
		errno = 0;
		return std::make_tuple(true, info);
	}
	else if (statusCode == "400") 
	{
		if (command == "start")
		{
			print::warn("Sent bad request to database (error 400)", 
				__FILE__, __LINE__);
			
			// Signal error but continue.
			errno = 400;
			return std::make_tuple(true, info);
		}
		error::errDBBadRequest(info, __FILE__, __LINE__);
	}
	else if (statusCode == "500") 
	{
		if (command == "start")
		{			
			print::warn("Internal error occurred in the database API (error 500)", 
				__FILE__, __LINE__);
			
			// Signal error but continue.
			errno = 500;
			return std::make_tuple(false, info);
		}
		error::errDBInternalError(info, __FILE__, __LINE__);
	}
	else 
	{
		if (command == "start")
		{
			print::warn("Database responded in an unexpected way", 
				__FILE__, __LINE__);
			
			// Signal error but continue.
			errno = EDOM;
			return std::make_tuple(false, info);
		}
		error::errDBUnknownResponse(__FILE__, __LINE__);
	}
}

NetworkHandler *DatabaseRequests::startConnection(EnvironmentDTO *env)
{
	NetworkHandler* nh = NetworkHandler::createHandler();
	nh->openConnection(env);
	return nh;
}
