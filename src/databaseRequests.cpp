/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "databaseRequests.h"
#include "networkUtils.h"
#include "networking.h"
#include "print.h"
#include "utils.h"

// Parser includes
#include "Parser.h"


std::string DatabaseRequests::uploadHashes(std::vector<HashData> &hashes, 
	ProjectMetaData metaData, 
	AuthorData &authorData, 
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAllDataFromHashes(hashes, dataSize, metaData.getAsHeader(), authorData);
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
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAllDataFromHashes(hashes, dataSize, metaData.getAsHeader(), authorData);

	return execRequest(DATABASE_CHECK_UPLOAD_REQUEST, rawData, dataSize, env);
}

std::string DatabaseRequests::getAuthor(const std::map<std::string, int> &authors, 
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAuthorStringToSend(authors, dataSize);

	return execRequest(DATABASE_GET_AUTHORS_BY_ID, rawData, dataSize, env);
}

std::string DatabaseRequests::getProjectData(const std::map<std::pair<std::string, std::string>, int> &project,
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getProjectsRequest(project, dataSize);

	return execRequest(DATABASE_GET_PROJECT_DATA, rawData, dataSize, env);
}

std::string DatabaseRequests::getNextJob(EnvironmentDTO *env)
{
	return execRequest(DATABASE_GET_NEXT_JOB, nullptr, 0, env);
}

std::string DatabaseRequests::addJobs(
	const std::vector<std::string>& jobs,
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getJobsRequest(jobs, dataSize);
	return execRequest(DATABASE_ADD_JOB, rawData, dataSize, env);
}

std::string DatabaseRequests::addCrawledJobs(
	const CrawlData& jobs,
	EnvironmentDTO *env)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getUploadCrawlRequest(jobs, dataSize);
	return execRequest(DATABASE_CRAWL_DATA, rawData, dataSize, env);
}

std::string DatabaseRequests::execRequest(
	std::string request, 
	const char* rawData, 
	int dataSize, 
	EnvironmentDTO *env)
{
	// First start the connection.
	NetworkHandler* networkHandler = startConnection(env->databaseAPIIP, env->databaseAPIPort);

	// Then send the header (what request we are doing and how much data we are sending).
	std::string requestType = request + std::to_string(dataSize) + "\n";
	networkHandler->sendData(requestType);

	// After that we send the data.
	networkHandler->sendData(rawData, dataSize);

	// Then we wait for the output that the API gives us.
	std::string output = networkHandler->receiveData();

	// Deleting the data we send and closing the connection.
	delete[] rawData;
	delete networkHandler;

	return checkResponseCode(output, env->commandString);
}

std::string DatabaseRequests::checkResponseCode(std::string data, std::string command)
{
	std::string statusCode = utils::split(data, '\n')[0];
	
	std::string info = "";
	if (data.length() >= 4) 
	{
		info = data.substr(4, data.length() - 4);
	}

	if (statusCode == "200") 
	{
		print::log("Database request successful.", __FILE__, __LINE__);
		return info;
	}
	else if (statusCode == "400") 
	{
		error::errDBBadRequest(info, __FILE__, __LINE__);
	}
	else if (statusCode == "500") 
	{
		error::errDBInternalError(info, __FILE__, __LINE__);
	}
	else 
	{
		error::errDBUnknownResponse(__FILE__, __LINE__);
	}
}

NetworkHandler* DatabaseRequests::startConnection(std::string apiIP, std::string apiPort)
{
	NetworkHandler* nh = NetworkHandler::createHandler();
	nh->openConnection(apiIP, apiPort);
	return nh;
}
