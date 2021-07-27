/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "commands.h"
#include "databaseRequests.h"
#include "moduleFacades.h"
#include "print.h"
#include "termination.h"
#include "utils.h"

// External includes.
#include <iostream>
#include <thread>

#define DOWNLOAD_LOCATION "spiderDownloads"


std::string Command::helpMessage()
{
	return helpMessageText;
}

#pragma region Start

Start::Start()
{
	this->helpMessageText = R"(
	start: Starts a workernode which will repeatedly execute jobs from the job queue.
	Can be stopped by typing "stop" in the command line. The worker node exits once it's done with its current job.
		Optionals :
			-c --cpu : Number of threads used by the worker node (minimum 2, default half).)";
}

void Start::logPreExecutionMessage(int fCPU, const char* file, int line)
{
	auto msg = "Starting a worker node with "
		+ std::to_string(fCPU) + " cpu cores";
	print::log(msg, file, line);
}

void Start::logPostExecutionMessage(const char* file, int line)
{
	print::log("Successfully terminated the worker node", file, line);
}

void Start::execute(Flags flags, EnvironmentDTO *env)
{
	if (flags.flag_github_token == "" || flags.flag_github_user == "")
	{
		error::errMissingGithubAuth(__FILE__, __LINE__);
	}

	logPreExecutionMessage(flags.flag_cpu, __FILE__, __LINE__);

	bool s = stop;
	std::thread t(&Start::readCommandLine, this);

	// Ask for jobs as long as the user does not want to stop.
	while (!s)
	{
		std::string job = DatabaseRequests::getNextJob(env);

		std::vector<std::string> splitted = Utils::split(job, INNER_DELIMITER);
		if (splitted.size() < 1)
		{
			error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
		}
		if (splitted[0] == "Spider")
		{
			print::log("New job: Download and parse " + splitted[1], __FILE__, __LINE__);
			versionProcessing(splitted, flags, env);
		}
		else if (splitted[0] == "Crawl")
		{
			print::log("New job: Crawl for more repository URLs", __FILE__, __LINE__);
			handleCrawlRequest(splitted, flags, env);
		}
		else if (splitted[0] == "NoJob")
		{
			print::debug("Waiting for a job to be available", __FILE__, __LINE__);
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
		else
		{
			error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
		}

		// Check if we need to stop.
		mtx.lock();
		s = stop;
		mtx.unlock();
	}
	t.join();
	logPostExecutionMessage(__FILE__, __LINE__);
}

void Start::handleCrawlRequest(std::vector<std::string> &splitted, Flags flags, EnvironmentDTO *env)
{
	if (splitted.size() < 2)
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	CrawlData crawled = moduleFacades::crawlRepositories(std::stoi(splitted[1]), flags);
	DatabaseRequests::addCrawledJobs(crawled, env);
}

void Start::handleSpiderRequest(std::vector<std::string> &splitted, Flags flags, EnvironmentDTO *env)
{
	Upload upload = Upload();
	if (splitted.size() < 2 || splitted[1] == "")
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	flags.mandatoryArgument = splitted[1];
	errno = 0;
	ProjectMetaData meta = moduleFacades::getProjectMetadata(flags.mandatoryArgument, flags);
	
	warnAndReturnIfErrno("Error getting project meta data, moving on to the next job.");

	flags.flag_branch = meta.defaultBranch;
	auto [authorData, commitHash, unchangedFiles] = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);
	
	warnAndReturnIfErrno("Error downloading project, moving on to the next job.");
	
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
	
	warnAndReturnIfErrno("Error parsing project, moving on to the next job.");
	
	if (hashes.size() == 0)
	{
		return;
	}

	// Uploading the hashes.
	print::debug(DatabaseRequests::uploadHashes(hashes, meta, authorData, env), __FILE__, __LINE__);
	print::log("Finished " + splitted[1], __FILE__, __LINE__);
}

void Start::readCommandLine()
{
	while (true)
	{
		std::string command;
		std::cin >> command;
		if (command == "stop")
		{
			mtx.lock();
			stop = true;
			mtx.unlock();
			break;
		}
	}
}

void Start::versionProcessing(std::vector<std::string>& splitted, Flags flags, EnvironmentDTO* env)
{
	if (splitted.size() < 2 || splitted[1] == "")
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	flags.mandatoryArgument = splitted[1];

	long long startingTime = 0; // Time to start from, request from db.

	// Get project metadata.
	ProjectMetaData meta = moduleFacades::getProjectMetadata(flags.mandatoryArgument, flags);
	
	warnAndReturnIfErrno("Error getting project meta data, moving on to the next job.");
	
	flags.flag_branch = meta.defaultBranch;

	
	std::pair<std::string, std::string> project{ meta.id, meta.versionTime };
	startingTime = DatabaseRequests::getProjectVersion(project, env);

	if (std::stoll(meta.versionTime) <= startingTime)
	{
		print::log("Most recent version of project already in database.", __FILE__, __LINE__);
		return;
	}

	// Download most recent commit, to retrieve tags.
	auto [authorData, commitHash, unchangedFiles] = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);
	
	warnAndReturnIfErrno("Error downloading project, moving on to the next job.");
	
	// Set commit hash of retrieved commit.
	meta.versionHash = commitHash;
	
	// Get tags of previous versions.
	std::vector<std::pair<std::string, long long>> tags = moduleFacades::getRepositoryTags(DOWNLOAD_LOCATION);
	
	warnAndReturnIfErrno("Error retrieving tags for project, just using most recent version.");
	
	// This is the first version and there are no tags, 
	// we just need to parse the most recent version we downloaded earlier.
	auto tagc = tags.size();

	print::log("Project has " + std::to_string(tagc) + print::plural(" tag", tagc), __FILE__, __LINE__);
	if (std::stoll(meta.versionTime) > startingTime && tagc == 0) 
	{		
		parseLatest(meta, authorData, flags, env);
	} 
	else if (tags.size() != 0) 
	{
		if (tags[tags.size() - 1].second <= startingTime)
		{
			print::log("Latest tag of project already in database.", __FILE__, __LINE__);
			return;
		}
		loopThroughTags(tags, meta, startingTime, flags, env);
	}
}

void Start::parseLatest(ProjectMetaData& meta, AuthorData& authorData, Flags& flags, EnvironmentDTO* env)
{
	print::debug("No tags found for project, just looking at HEAD.", __FILE__, __LINE__);
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);

	warnAndReturnIfErrno("Error parsing project, moving on to the next job.");

	if (hashes.size() == 0)
	{
		return;
	}

	print::debug(DatabaseRequests::uploadHashes(hashes, meta, authorData, env), __FILE__, __LINE__);
}

void Start::loopThroughTags(
	std::vector<std::pair<std::string, long long>>& tags,
	ProjectMetaData &meta,
	long long startingTime,
	Flags &flags,
	EnvironmentDTO* env)
{

	std::string prevTag = tags[0].first;
	std::string prevVersionTime = "";

	print::debug("Starting with tag: " + prevTag + ".", __FILE__, __LINE__);

	for (int i = 0; i < tags.size(); i++)
	{
		std::string curTag = tags[i].first;
		long long versionTime = tags[i].second; // Update the time of this commit.

		// Skip tags before the starting time.
		if (versionTime <= startingTime)
		{
			prevTag = curTag;
			prevVersionTime = std::to_string(versionTime);
			continue;
		}
		meta.versionTime = std::to_string(versionTime);

		print::log("Processing tag: " + curTag + "(" + std::to_string(i + 1) + "/" + std::to_string(tags.size()) + ")",
				   __FILE__, __LINE__);

		print::debug("Comparing tags: " + prevTag + " and " + curTag + ".", __FILE__, __LINE__);

		downloadTagged(flags, prevTag, curTag, meta, prevVersionTime, env);

		prevTag = curTag;
		prevVersionTime = std::to_string(versionTime);
	}
}

void Start::downloadTagged(Flags flags, std::string prevTag, std::string curTag, ProjectMetaData meta, std::string prevVersionTime, EnvironmentDTO* env)
{
	auto [authorData, commitHash, unchangedFiles] = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION, prevTag, curTag);
	
	warnAndReturnIfErrno("Error downloading tagged version of project, moving on to the next tag.");
	
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
	
	warnAndReturnIfErrno("Error parsing tagged version of project, moving on to the next tag.");
	

	meta.versionHash = commitHash;

	// Uploading the hashes.
	print::debug(DatabaseRequests::uploadHashes(hashes, meta, authorData, env, prevVersionTime, unchangedFiles),
		__FILE__, __LINE__);

}

#pragma endregion Start

#pragma region Check

Check::Check()
{
	this->helpMessageText = R"(
	check: Hashes all methods from a github repository, and checks for matches with the database.
	A Summary will be printed to the console and written to a file.
		Arguments:
			Url to a github repository.
		Optionals :
			-b --branch : The branch to parse.
			-c --cpu : Number of threads used (minimum 2, default half).)";
}

std::string Check::partialLogMessage(std::string url)
{
	return " the code from the project at " + url + " against the SearchSECO database";
}

void Check::logPreExecutionMessage(std::string url, const char* file, int line)
{
	print::log("Checking" + Check::partialLogMessage(url), file, line);
}

void Check::logPostExecutionMessage(std::string url, const char* file, int line)
{
	print::log("Successfully checked" + Check::partialLogMessage(url), file, line);
}

void Check::execute(Flags flags, EnvironmentDTO *env)
{
	auto url = flags.mandatoryArgument;

	this->logPreExecutionMessage(url, __FILE__, __LINE__);

	auto [authorData, commitHash, unchangedFiles] = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
	if (errno != 0)
	{
		termination::failureSpider(__FILE__, __LINE__);
	}
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
	if (errno != 0)
	{
		termination::failureParser(__FILE__, __LINE__);
	}

	// Calling the function that will print all the matches for us.
	PrintMatches::printHashMatches(
		hashes, 
		DatabaseRequests::findMatches(
			hashes, 
			env
		), 
		authorData, 
		env,
		url
	);

	this->logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion Check

#pragma region Upload

Upload::Upload()
{
	this->helpMessageText = R"(
	check: Hashes all methods from a github repository, and uploads them to the database.
		Arguments:
			Url to a github repository.
		Optionals :
			-b --branch : The branch to parse.
			-c --cpu : Number of threads used (minimum 2, default half).)";
}

std::string Upload::partialLogMessage(std::string url)
{
	return " the code from the project at " + url + " to the SearchSECO database";
}

void Upload::logPreExecutionMessage(std::string url, const char* file, int line)
{
	print::log("Uploading" + Upload::partialLogMessage(url), file, line);
}

void Upload::logPostExecutionMessage(std::string url, const char* file, int line)
{
	print::log("Successfully uploaded" + Upload::partialLogMessage(url), file, line);
}

void Upload::execute(Flags flags, EnvironmentDTO *env)
{
	if (flags.flag_github_token == "" || flags.flag_github_user == "")
	{
		error::errMissingGithubAuth(__FILE__, __LINE__);
		return;
	}

	auto url = flags.mandatoryArgument;

	this->logPreExecutionMessage(url, __FILE__, __LINE__);

	auto [authorData, commitHash, unchangedFiles] = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
	if (errno != 0)
	{
		termination::failureSpider(__FILE__, __LINE__);
	}
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
	if (errno != 0)
	{
		termination::failureParser(__FILE__, __LINE__);
	}
	// Uploading the hashes.
	ProjectMetaData meta = moduleFacades::getProjectMetadata(url, flags);
	if (errno != 0)
	{
		termination::failureCrawler(__FILE__, __LINE__);
	}
	meta.versionHash = commitHash;
	print::log(DatabaseRequests::uploadHashes(hashes, meta, authorData, env), __FILE__, __LINE__);

	this->logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion Upload

#pragma region CheckUpload

CheckUpload::CheckUpload()
{
	this->helpMessageText = R"(
	checkupload: Hashes all methods from a github repository, and both checks and uploads them.
		Arguments:
			Url to a github repository.
		Optionals :
			-b --branch : The branch to parse.
			-c --cpu : Number of threads used (minimum 2, default half).)";
}

void CheckUpload::execute(Flags flags, EnvironmentDTO *env)
{
	if (flags.flag_github_token == "" || flags.flag_github_user == "")
	{
		error::errMissingGithubAuth(__FILE__, __LINE__);
		return;
	}

	auto url = flags.mandatoryArgument;

	Check::logPreExecutionMessage(url, __FILE__, __LINE__);

	auto [authorData, commitHash, unchangedFiles] = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
	if (errno != 0)
	{
		termination::failureSpider(__FILE__, __LINE__);
	}
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
	if (errno != 0)
	{
		termination::failureParser(__FILE__, __LINE__);
	}

	Check::logPostExecutionMessage(url, __FILE__, __LINE__);


	Upload::logPreExecutionMessage(url, __FILE__, __LINE__);

	ProjectMetaData metaData = moduleFacades::getProjectMetadata(url, flags);
	if (errno != 0)
	{
		termination::failureCrawler(__FILE__, __LINE__);
	}
	metaData.versionHash = commitHash;

	PrintMatches::printHashMatches(
		hashes, 
		DatabaseRequests::checkUploadHashes(hashes, metaData, authorData, env), 
		authorData, 
		env,
		url
	);
	
	Upload::logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion CheckUpload
