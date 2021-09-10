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

std::tuple<std::vector<HashData>, AuthorData> Command::parseAndBlame(Spider *s, Flags flags)
{
	// Parse all parseable files.
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);

	if (errno != 0)
	{
		print::warn("Error parsing project.", __FILE__, __LINE__);
		return std::tuple<std::vector<HashData>, AuthorData>(std::vector<HashData>(), AuthorData());
	}

	// If no methods were found, we do not need to retrieve any author data.
	if (hashes.size() == 0)
	{
		print::debug("No methods present, skipping authors", __FILE__, __LINE__);
		return std::tuple<std::vector<HashData>, AuthorData>(hashes, AuthorData());
	}

	// Retrieve author data.
	AuthorData authorData = moduleFacades::getAuthors(s, DOWNLOAD_LOCATION);

	if (errno != 0)
	{
		print::warn("Error retrieving author data.", __FILE__, __LINE__);
		return std::tuple<std::vector<HashData>, AuthorData>(std::vector<HashData>(), AuthorData());
	}

	// Return the extracted data.
	return std::tuple<std::vector<HashData>, AuthorData>(hashes, authorData);
}

void Command::uploadProject(Spider *s, Flags flags, ProjectMetaData meta, EnvironmentDTO *env)
{
	long long startingTime = 0; // Time to start from, request from db.

	// Find most newest version of project in database.
	std::pair<std::string, std::string> project{meta.id, meta.versionTime};
	startingTime = DatabaseRequests::getProjectVersion(project, env);

	if (std::stoll(meta.versionTime) <= startingTime)
	{
		print::log("Most recent version of project already in database.", __FILE__, __LINE__);
		return;
	}

	meta.versionHash = moduleFacades::currentVersion(s, DOWNLOAD_LOCATION);

	warnAndReturnIfErrno("Error downloading project, moving on to the next job.");

	// Get tags of previous versions.
	std::vector<std::tuple<std::string, long long, std::string>> tags =
		moduleFacades::getRepositoryTags(DOWNLOAD_LOCATION);

	warnAndReturnIfErrno("Error retrieving tags for project, just using most recent version.");

	// This is the first version and there are no tags,
	// we just need to parse the most recent version we downloaded earlier.
	auto tagc = tags.size();

	print::log("Project has " + std::to_string(tagc) + print::plural(" tag", tagc), __FILE__, __LINE__);
	if (std::stoll(meta.versionTime) > startingTime && tagc == 0)
	{
		parseLatest(s, meta, flags, env);
	}
	else if (tagc != 0)
	{
		if (std::get<1>(tags[tagc - 1]) <= startingTime)
		{
			print::log("Latest tag of project already in database.", __FILE__, __LINE__);
			return;
		}
		loopThroughTags(s, tags, meta, startingTime, flags, env);
	}
}

void Command::parseLatest(Spider *s, ProjectMetaData &meta, Flags &flags, EnvironmentDTO *env)
{
	print::debug("No tags found for project, just looking at HEAD.", __FILE__, __LINE__);

	auto [hashes, authorData] = Command::parseAndBlame(s, flags);
	warnAndReturnIfErrno("Skipping project.");

	if (hashes.size() == 0)
	{
		return;
	}

	print::debug(DatabaseRequests::uploadHashes(hashes, meta, authorData, env), __FILE__, __LINE__);
}

void Command::loopThroughTags(Spider *s, std::vector<std::tuple<std::string, long long, std::string>> &tags,
							ProjectMetaData &meta, long long startingTime, Flags &flags, EnvironmentDTO *env)
{
	// Skip tags before the starting time.
	int i = 0;
	while (std::get<1>(tags[i]) <= startingTime)
	{
		i++;
	}

	// Revert to oldest unprocessed tag.
	std::string prevTag = "";
	std::string prevVersionTime = "";
	std::vector<std::string> prevUnchangedFiles = std::vector<std::string>();

	if (i > 0)
	{
		std::string prevTag = std::get<0>(tags[i - 1]);
		std::string prevVersionTime = std::to_string(std::get<1>(tags[i - 1]));
	}

	moduleFacades::switchVersion(s, "HEAD", DOWNLOAD_LOCATION);

	// Loop through remaining tags.
	for (; i < tags.size(); i++)
	{
		std::string curTag = std::get<0>(tags[i]);
		long long versionTime = std::get<1>(tags[i]);
		std::string versionHash = std::get<2>(tags[i]);

		meta.versionTime = std::to_string(versionTime);
		meta.versionHash = versionHash;

		// Log information.
		print::log("Processing tag: " + curTag + "(" + std::to_string(i + 1) + "/" + std::to_string(tags.size()) + ")",
				   __FILE__, __LINE__);

		print::debug("Comparing tags: " + prevTag + " and " + curTag + ".", __FILE__, __LINE__);

		downloadTagged(s, flags, prevTag, curTag, meta, prevVersionTime, prevUnchangedFiles, env);

		prevTag = curTag;
		prevVersionTime = std::to_string(versionTime);
	}
}

void Command::downloadTagged(Spider *s, Flags flags, std::string prevTag, std::string curTag, ProjectMetaData meta,
							 std::string prevVersionTime, std::vector<std::string> &prevUnchangedFiles,
							 EnvironmentDTO *env)
{
	std::vector<std::string> unchangedFiles =
		moduleFacades::updateVersion(s, DOWNLOAD_LOCATION, prevTag, curTag, prevUnchangedFiles);

	warnAndReturnIfErrno("Error downloading tagged version of project, moving on to the next tag.");

	auto [hashes, authorData] = Command::parseAndBlame(s, flags);
	warnAndReturnIfErrno("Skipping project.");

	// Uploading the hashes.
	print::debug(DatabaseRequests::uploadHashes(hashes, meta, authorData, env, prevVersionTime, unchangedFiles),
				 __FILE__, __LINE__);

	prevUnchangedFiles = unchangedFiles;
}

#pragma region Start

Start::Start()
{
	this->helpMessageText = R"(
	start: Starts a worker node which will repeatedly execute jobs from the job queue.
	Can be stopped by typing "stop" in the command line. The worker node exits once it is done with its current job.
		Optionals :
			-c --cpu : Number of threads used by the worker node (minimum 2, default half).)";
}

void Start::logPreExecutionMessage(int fCPU, const char *file, int line)
{
	auto msg = "Starting a worker node with " + std::to_string(fCPU) + " cpu cores";
	print::log(msg, file, line);
}

void Start::logPostExecutionMessage(const char *file, int line)
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
	if (splitted.size() < 3)
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	CrawlData crawled = moduleFacades::crawlRepositories(std::stoi(splitted[1]), flags);
	DatabaseRequests::addCrawledJobs(crawled, splitted[2], env);
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

void Start::versionProcessing(std::vector<std::string> &splitted, Flags flags, EnvironmentDTO *env)
{
	if (splitted.size() < 2 || splitted[1] == "")
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	flags.mandatoryArgument = splitted[1];

	// Get project metadata.
	ProjectMetaData meta = moduleFacades::getProjectMetadata(flags.mandatoryArgument, flags);

	warnAndReturnIfErrno("Error getting project meta data, moving on to the next job.");

	if (flags.flag_branch == "")
	{
		// Set default branch.
		flags.flag_branch = meta.defaultBranch;
	}

	// Initialize spider.
	Spider *s = moduleFacades::setupSpider(flags.mandatoryArgument, flags);

	warnAndReturnIfErrno("No suitable Spider, skipping project.");

	// Download project.
	moduleFacades::downloadRepo(s, flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);

	// Process and upload project.
	Command::uploadProject(s, flags, meta, env);
}

#pragma endregion Start

#pragma region Check

Check::Check()
{
	this->helpMessageText = R"(
	check: Hashes all methods from a github repository, and checks for matches with the database.
	A Summary will be printed to the console and written to a file. Only checks methods in the most recent version of the repository.
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

void Check::logPreExecutionMessage(std::string url, const char *file, int line)
{
	print::log("Checking" + Check::partialLogMessage(url), file, line);
}

void Check::logPostExecutionMessage(std::string url, const char *file, int line)
{
	print::log("Successfully checked" + Check::partialLogMessage(url), file, line);
}

void Check::execute(Flags flags, EnvironmentDTO *env)
{
	auto url = flags.mandatoryArgument;

	this->logPreExecutionMessage(url, __FILE__, __LINE__);

	// Get project metadata.
	ProjectMetaData meta = moduleFacades::getProjectMetadata(flags.mandatoryArgument, flags);

	warnAndReturnIfErrno("Error getting project meta data, moving on to the next job.");

	if (flags.flag_branch == "")
	{
		// Set default branch.
		flags.flag_branch = meta.defaultBranch;
	}

	// Initialize spider.
	Spider *s = moduleFacades::setupSpider(url, flags);

	warnAndReturnIfErrno("No suitable Spider, skipping project.");

	// Download project.
	moduleFacades::downloadRepo(s, url, flags, DOWNLOAD_LOCATION);

	auto [hashes, authorData] = Command::parseAndBlame(s, flags);
	warnAndReturnIfErrno("Error processing project.");

	// Calling the function that will print all the matches for us.
	PrintMatches::printHashMatches(hashes, DatabaseRequests::findMatches(hashes, env), authorData, env, url);

	this->logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion Check

#pragma region Upload

Upload::Upload()
{
	this->helpMessageText = R"(
	upload: Hashes all methods from all versions of a github repository, and uploads them to the database.
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

void Upload::logPreExecutionMessage(std::string url, const char *file, int line)
{
	print::log("Uploading" + Upload::partialLogMessage(url), file, line);
}

void Upload::logPostExecutionMessage(std::string url, const char *file, int line)
{
	print::log("Successfully uploaded" + Upload::partialLogMessage(url), file, line);
}

void Upload::execute(Flags flags, EnvironmentDTO *env)
{
	this->logPreExecutionMessage(flags.mandatoryArgument, __FILE__, __LINE__);

	// Get project metadata.
	ProjectMetaData meta = moduleFacades::getProjectMetadata(flags.mandatoryArgument, flags);

	warnAndReturnIfErrno("Error getting project meta data, moving on to the next job.");

	if (flags.flag_branch == "")
	{
		// Set default branch.
		flags.flag_branch = meta.defaultBranch;
	}

	// Initialize spider.
	Spider *s = moduleFacades::setupSpider(flags.mandatoryArgument, flags);

	warnAndReturnIfErrno("No suitable Spider, skipping project.");

	// Download project.
	moduleFacades::downloadRepo(s, flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);

	// Process and upload project.
	Command::uploadProject(s, flags, meta, env);
	this->logPostExecutionMessage(flags.mandatoryArgument, __FILE__, __LINE__);
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

	// Get project metadata.
	ProjectMetaData meta = moduleFacades::getProjectMetadata(flags.mandatoryArgument, flags);

	warnAndReturnIfErrno("Error getting project meta data, moving on to the next job.");

	if (flags.flag_branch == "")
	{
		// Set default branch.
		flags.flag_branch = meta.defaultBranch;
	}

	// Initialize spider.
	Spider *s = moduleFacades::setupSpider(url, flags);

	warnAndReturnIfErrno("No suitable Spider, skipping project.");

	// Download project.
	moduleFacades::downloadRepo(s, url, flags, DOWNLOAD_LOCATION);

	auto [hashes, authorData] = Command::parseAndBlame(s, flags);
	warnAndReturnIfErrno("Error processing project.");

	// Calling the function that will print all the matches for us.
	PrintMatches::printHashMatches(hashes, DatabaseRequests::findMatches(hashes, env), authorData, env, url);

	Check::logPostExecutionMessage(url, __FILE__, __LINE__);

	// Ugly, but download project again, otherwise we have metadata in the folder already.
	moduleFacades::downloadRepo(s, url, flags, DOWNLOAD_LOCATION);

	Upload::logPreExecutionMessage(url, __FILE__, __LINE__);

	Command::uploadProject(s, flags, meta, env);

	Upload::logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion CheckUpload
