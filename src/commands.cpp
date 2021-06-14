/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "commands.h"
#include "databaseRequests.h"
#include "flags.h"
#include "moduleFacades.h"
#include "print.h"

// Parser includes.
#include "Parser.h"

// Spider includes.
#include "RunSpider.h"

#include <iostream>
#include <thread>
#include "termination.h"


#define DOWNLOAD_LOCATION "spiderDownloads"

std::string Command::helpMessage()
{
	return helpMessageText;
}

#pragma region Start

Start::Start()
{
	this->helpMessageText = R"(
	start: Starts a workernode.
		Arguments:
		Storage size(in GB)
			Location where the local database is stored.
			Optionals :
			-c --cpu : Number of threads used(minimum 2, default half)
			-r --ram : RAM cap(in GB) (default no cap))";
}

void Start::logPreExecutionMessage(int fCPU, const char* file, int line)
{
	auto msg = "Starting a worker node with "
		+ std::to_string(fCPU) + " cpu cores";
	print::log(msg, file, line);
}

void Start::logPostExecutionMessage(const char* file, int line)
{
	print::loguruResetThreadName();
	print::log("Successfully terminated the worker node", file, line);
}

void Start::execute(Flags flags)
{
	if (flags.flag_github_token == "" || flags.flag_github_user == "")
	{
		error::errMissingGithubAuth(__FILE__, __LINE__);
		return;
	}

	logPreExecutionMessage(flags.flag_cpu, __FILE__, __LINE__);

	bool s = stop;
	std::thread t(&Start::readCommandLine, this);
	while (!s)
	{
		std::string job = DatabaseRequests::getNextJob();

		std::vector<std::string> splitted = utils::split(job, INNER_DELIMITER);
		if (splitted.size() < 1)
		{
			error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
		}
		if (splitted[0] == "Spider")
		{
			handleSpiderRequest(splitted, flags);
		}
		else if (splitted[0] == "Crawl")
		{
			handleCrawlRequest(splitted, flags);
		}
		else if (splitted[0] == "NoJob")
		{
			print::log("Waiting for a job to be available", __FILE__, __LINE__);
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

void Start::handleCrawlRequest(std::vector<std::string>& splitted, Flags flags)
{
	print::log("Start crawling", __FILE__, __LINE__);
	if (splitted.size() < 2)
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	CrawlData crawled = moduleFacades::crawlRepositories(std::stoi(splitted[1]), flags);
	DatabaseRequests::addCrawledJobs(crawled);
}

void Start::handleSpiderRequest(std::vector<std::string>& splitted, Flags flags)
{
	print::log("Start parsing and uploading " + splitted[1], __FILE__, __LINE__);
	Upload upload = Upload();
	if (splitted.size() < 2 || splitted[1] == "")
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	flags.mandatoryArgument = splitted[1];
	errno = 0;
	ProjectMetaData meta = moduleFacades::getProjectMetadata(flags.mandatoryArgument, flags);
	if (errno != 0)
	{
		errno = 0;
		print::warn("Error getting project meta data, moving on to the next job.", __FILE__, __LINE__);
		return;
	}
	flags.flag_branch = meta.defaultBranch;
	AuthorData authorData = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);
	if (errno != 0)
	{
		errno = 0;
		print::warn("Error downloading project, moving on to the next job.", __FILE__, __LINE__);
		return;
	}
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
	if (errno != 0)
	{
		errno = 0;
		print::warn("Error parsing project, moving on to the next job.", __FILE__, __LINE__);
		return;
	}
	if (hashes.size() == 0)
	{
		return;
	}
	// Uploading the hashes.
	print::printline(DatabaseRequests::uploadHashes(hashes, meta, authorData));
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

void Start::versionProcessing(std::vector<std::string>& splitted, Flags flags)
{
	std::string startingTag = ""; // Tag to start from
	bool startingTagReached = false;
	if (startingTag == "")
	{
		startingTagReached = true;
	}

	AuthorData authorData = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);
	std::vector<std::pair<std::string, long long>> tags = moduleFacades::getRepositoryTags(DOWNLOAD_LOCATION);

	std::string prevTag = "";

	for (int i = tags.size() - 1; i >= 0; i--)
	{
		std::string curTag = tags[i].first;

		if (!startingTagReached && curTag != startingTag)
		{
			continue;
		}
		else if (curTag == startingTag)
		{
			startingTagReached = true;
		}

		AuthorData authorData = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION, prevTag, curTag);
		if (errno != 0)
		{
			errno = 0;
			print::warn("Error downloading tagged version of project, moving on to the next tag.", __FILE__, __LINE__);
			continue;
		}
		std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
		if (errno != 0)
		{
			errno = 0;
			print::warn("Error parsing tagged version of project, moving on to the next tag.", __FILE__, __LINE__);
			continue;
		}
		if (hashes.size() == 0)
		{
			continue;
		}
		//handling unchanged files?

		// Uploading the hashes.

		prevTag = curTag;
	}
}

#pragma endregion Start

#pragma region Check

Check::Check()
{
	this->helpMessageText = R"(
	check: Takes a github url, downloads it and parses it. After that it will check for matches with the database.  
		Arguments:
			Url to a github repository.
		Optionals:
			-o --output: Console to print to the console, else you can give a file path.  
			-s --save: Save the parser results for later use.)";
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
	print::loguruResetThreadName();
	print::log("Successfully checked" + Check::partialLogMessage(url), file, line);
}

void Check::execute(Flags flags)
{
	auto url = flags.mandatoryArgument;

	this->logPreExecutionMessage(url, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
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
	printMatches::printHashMatches(hashes, DatabaseRequests::findMatches(hashes), authorData, url);
	//TODO: delete temp folder.

	this->logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion Check

#pragma region Upload

Upload::Upload()
{
	this->helpMessageText = R"(
	upload: Takes a github url, downloads it and parses it. Results are sent to the database.
		Arguments:
			Url to a github repository.  
		Optionals:
			-s --save: Save the parser results for later use.)";
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
	print::loguruResetThreadName();
	print::log("Successfully uploaded" + Upload::partialLogMessage(url), file, line);
}

void Upload::execute(Flags flags)
{
	if (flags.flag_github_token == "" || flags.flag_github_user == "")
	{
		error::errMissingGithubAuth(__FILE__, __LINE__);
		return;
	}

	auto url = flags.mandatoryArgument;

	this->logPreExecutionMessage(url, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
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
	print::printline(DatabaseRequests::uploadHashes(hashes, meta, authorData));

	this->logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion Upload

#pragma region CheckUpload

CheckUpload::CheckUpload()
{
	this->helpMessageText = R"(
	checkupload: Takes a github url, and does both the check and upload.
		Arguments:
			Url to a github repository.
		Optionals:
			-o --output: Console to print to the console, else you can give a file path.  
			-s --save: Save the parser results for later use.)";
}

void CheckUpload::execute(Flags flags)
{
	if (flags.flag_github_token == "" || flags.flag_github_user == "")
	{
		error::errMissingGithubAuth(__FILE__, __LINE__);
		return;
	}

	auto url = flags.mandatoryArgument;

	Check::logPreExecutionMessage(url, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
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

	printMatches::printHashMatches(
		hashes, 
		DatabaseRequests::checkUploadHashes(hashes, metaData, authorData), 
		authorData, 
		url
	);

	Upload::logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion CheckUpload
