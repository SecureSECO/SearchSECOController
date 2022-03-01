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
#include <atomic>
#include <condition_variable>
#include <climits>

std::atomic<bool> stopped(false);

std::string Command::helpMessage()
{
	return helpMessageText;
}

std::tuple<std::vector<HashData>, AuthorData> Command::parseAndBlame(Spider *s, std::string tag, std::string jobid,
																	 std::string &jobTime, Flags flags,
																	 EnvironmentDTO *env)
{
	// Parse all parseable files.
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);

	if (errno != 0 && !stopped)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::parser,
									"Error parsing tag \"" + tag + "\": " + std::to_string(errno) + ".", env);
		print::warn("Error parsing project.", __FILE__, __LINE__);
		return std::tuple<std::vector<HashData>, AuthorData>(std::vector<HashData>(), AuthorData());
	}
	if (stopped)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::timeout,
									"Timeout hit during parsing of tag  \"" + tag + "\".", env);
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
		DatabaseRequests::finishJob(
			jobid, jobTime, FinishReason::authorData,
			"Error retrieving author data for tag \"" + tag + "\": " + std::to_string(errno) + ".", env);
		print::warn("Error retrieving author data.", __FILE__, __LINE__);
		return std::tuple<std::vector<HashData>, AuthorData>(std::vector<HashData>(), AuthorData());
	}

	// Return the extracted data.
	return std::tuple<std::vector<HashData>, AuthorData>(hashes, authorData);
}

void Command::checkProject(Flags flags, EnvironmentDTO *env)
{
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

	if (flags.flag_projectCommit != "")
	{
		moduleFacades::switchVersion(s, flags.flag_projectCommit, DOWNLOAD_LOCATION);
	}

	std::string empty = "";
	auto [hashes, authorData] = Command::parseAndBlame(s, "HEAD", "0", empty, flags, env);
	warnAndReturnIfErrno("Error processing project.");

	// Calling the function that will print all the matches for us.
	PrintMatches::printHashMatches(hashes, DatabaseRequests::findMatches(hashes, env), authorData, env, url);

	Check::logPostExecutionMessage(url, __FILE__, __LINE__);
}

void Command::uploadProject(Flags flags, std::string jobid, std::string &jobTime,
							long long *startTime, EnvironmentDTO *env, std::string downloadLocation)
{
	// Get project metadata.
	ProjectMetaData meta = moduleFacades::getProjectMetadata(flags.mandatoryArgument, flags);

	if (errno != 0)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::projectMeta, "Error getting project meta data: " + std::to_string(errno) + ".", env);
	}
	warnAndReturnIfErrno("Error getting project meta data, moving on to the next job.");

	if (flags.flag_branch == "")
	{
		// Set default branch.
		flags.flag_branch = meta.defaultBranch;
	}

	// Initialize spider.
	Spider *s = moduleFacades::setupSpider(flags.mandatoryArgument, flags);

	if (errno != 0)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::spiderSetup, "No suitable spider: " + std::to_string(errno) + ".", env);
	}
	warnAndReturnIfErrno("No suitable Spider, skipping project.");

	long long startingTime = 0; // Time to start from, request from db.

	// Find most newest version of project in database.
	std::pair<std::string, std::string> project{meta.id, meta.versionTime};
	startingTime = DatabaseRequests::getProjectVersion(project, env);

	if (std::stoll(meta.versionTime) <= startingTime)
	{
		print::log("Most recent version of project already in database.", __FILE__, __LINE__);
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::alreadyKnown, "Project already known.", env);
		return;
	}

	meta.versionHash = moduleFacades::currentVersion(s, downloadLocation);

	// Download project.
	moduleFacades::downloadRepo(s, flags.mandatoryArgument, flags, downloadLocation);	

	if (errno != 0)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::projectDownload, "Error downloading project: " + std::to_string(errno) + ".", env);
	}
	if (stopped)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::timeout, "Job timed out on download.",
									env);
	}
	warnAndReturnIfErrno("Error downloading project, moving on to the next job.");

	// Get tags of previous versions.
	std::vector<std::tuple<std::string, long long, std::string>> tags =
		moduleFacades::getRepositoryTags(downloadLocation);

	if (errno != 0)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::tagRetrieval, "Error retrieving tags: " + std::to_string(errno) + ".", env);
	}
	warnAndReturnIfErrno("Error retrieving tags for project, moving on to the next job.");

	// This is the first version and there are no tags,
	// we just need to parse the most recent version we downloaded earlier.
	auto tagc = tags.size();

	print::log("Project has " + std::to_string(tagc) + print::plural(" tag", tagc), __FILE__, __LINE__);
	if (std::stoll(meta.versionTime) > startingTime && tagc == 0)
	{
		parseLatest(s, meta, jobid, jobTime, flags, env);
	}
	else if (tagc != 0)
	{
		if (std::get<1>(tags[tagc - 1]) <= startingTime)
		{
			print::log("Latest tag of project already in database.", __FILE__, __LINE__);
			DatabaseRequests::finishJob(jobid, jobTime, FinishReason::alreadyKnown, "Project already known.", env);
			return;
		}
		loopThroughTags(s, tags, meta, startingTime, flags, jobid, jobTime, startTime, env);
	}
}

void Command::parseLatest(Spider *s, ProjectMetaData &meta, std::string jobid, std::string &jobTime, Flags &flags,
						  EnvironmentDTO *env)
{
	print::debug("No tags found for project, just looking at HEAD.", __FILE__, __LINE__);

	auto [hashes, authorData] = Command::parseAndBlame(s, "HEAD", jobid, jobTime, flags, env);

	warnAndReturnIfErrno("Skipping project.");

	if (hashes.size() == 0)
	{
		return;
	}

	print::debug(DatabaseRequests::uploadHashes(hashes, meta, authorData, env), __FILE__, __LINE__);
	if (errno != 0)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::uploadHashes, "Error uploading hashes to database: " + std::to_string(errno) + ".", env);
	}
}

void Command::loopThroughTags(Spider *s, std::vector<std::tuple<std::string, long long, std::string>> &tags,
							ProjectMetaData &meta, long long startingTime, Flags &flags, std::string jobid, std::string &jobTime, long long *startTime, EnvironmentDTO *env)
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

		if (env->commandString == "start")
		{
			if (stopped)
			{
				DatabaseRequests::finishJob(jobid, jobTime, FinishReason::timeout,
											"Job timed out on tag: " + prevTag + ".",
											env);
				return;
			}

			DatabaseRequests::updateJob(jobid, jobTime, env);
			
			// If update was unsuccessful or unexpected,
			if (errno != 0)
			{
				// stop the current job.
				DatabaseRequests::finishJob(jobid, jobTime, FinishReason::jobUpdate,
											"Error receiving job update from database: " + std::to_string(errno) + ".",
											env);
				return;
			}
			*startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
							std::chrono::system_clock::now().time_since_epoch())
							.count();
		}

		downloadTagged(s, flags, prevTag, curTag, meta, prevVersionTime, prevUnchangedFiles, jobid, jobTime, env);

		prevTag = curTag;
		prevVersionTime = std::to_string(versionTime);

		if (errno != 0){
			// Stop processing tags if a certain tag failed.
			return;
		}
	}
}

void Command::downloadTagged(Spider *s, Flags flags, std::string prevTag, std::string curTag, ProjectMetaData meta,
							 std::string prevVersionTime, std::vector<std::string> &prevUnchangedFiles,
							 std::string jobid, std::string &jobTime, 
							 EnvironmentDTO *env)
{
	std::vector<std::string> unchangedFiles =
		moduleFacades::updateVersion(s, DOWNLOAD_LOCATION, prevTag, curTag, prevUnchangedFiles);

	if (errno != 0)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::tagUpdate, "Error updating from tag \"" + prevTag + "\" to tag \"" + curTag + "\": "+ std::to_string(errno) + ".", env);
	}
	warnAndReturnIfErrno("Error downloading tagged version of project, moving on to the next job.");

	auto [hashes, authorData] = Command::parseAndBlame(s, curTag, jobid, jobTime, flags, env);


	warnAndReturnIfErrno("Skipping project.");

	if (stopped)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::timeout,
									"Timeout hit on tag: " + curTag + ".", env);
		return;
	}

	// Uploading the hashes.
	print::debug(DatabaseRequests::uploadHashes(hashes, meta, authorData, env, prevVersionTime, unchangedFiles),
				 __FILE__, __LINE__);

	if (errno != 0)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::uploadHashes, "Error uploading hashes to database: " + std::to_string(errno) + ".", env);
	}

	prevUnchangedFiles = unchangedFiles;
}

void Command::uploadPartialProject(Flags flags, std::string version, std::map<std::string, std::vector<int>> lines,
								   std::string vulnCode, EnvironmentDTO *env)
{
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
	warnAndReturnIfErrno("No suitable Spider.");

	// Download project.
	moduleFacades::downloadRepo(s, flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);

	warnAndReturnIfErrno("Error downloading project.");

	moduleFacades::switchVersion(s, version, DOWNLOAD_LOCATION);

	moduleFacades::trimFiles(s, lines, DOWNLOAD_LOCATION);

	// Parse all parseable files.
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);

	if (errno != 0)
	{
		print::warn("Error parsing project.", __FILE__, __LINE__);
		return;
	}

	hashes = trimHashes(hashes, lines);

	// If no methods were found, we do not need to retrieve any author data.
	if (hashes.size() == 0)
	{
		print::debug("No methods present, skipping authors", __FILE__, __LINE__);
		return;
	}

	for (int i = 0; i < hashes.size(); i++)
	{
		hashes[i].vulnCode = vulnCode;
	}

	// Retrieve author data.
	AuthorData authorData = moduleFacades::getAuthors(s, DOWNLOAD_LOCATION);

	if (errno != 0)
	{
		print::warn("Error retrieving author data.", __FILE__, __LINE__);
		return;
	}

	print::debug(DatabaseRequests::uploadHashes(hashes, meta, authorData, env), __FILE__, __LINE__);
}

std::vector<HashData> Command::trimHashes(std::vector<HashData> hashes, std::map<std::string, std::vector<int>> lines)
{
	std::vector<HashData> result;
	for (HashData hash : hashes)
	{
		if (lines.count(hash.fileName) > 0)
		{
			for (int line : lines[hash.fileName])
			{
				if (hash.lineNumber <= line && line <= hash.lineNumberEnd)
				{
					result.push_back(hash);
					break;
				}
			}
		}
	}
	return result;
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

	// Prevent users from being able to parse different branches with the start command.
	flags.flag_branch = "";

	logPreExecutionMessage(flags.flag_cpu, __FILE__, __LINE__);

	std::thread t(&Start::readCommandLine, this);

	int threadCount = flags.flag_cpu / 2;
	flags.flag_cpu = 2;

	std::vector<std::thread> threads;

	for (int i = 0; i < threadCount; i++)
	{
		threads.push_back(std::thread(&Start::runStart, this, flags, i, env));
	}

	for (int i = 0; i < threadCount; i++)
	{
		threads[i].join();
	}
	
	t.join();
	logPostExecutionMessage(__FILE__, __LINE__);
}

void Start::runStart(Flags flags, int thread, EnvironmentDTO *env)
{
	loguru::set_thread_name(("controller"+std::to_string(thread)).c_str());
	bool s = stop;
	// Ask for jobs as long as the user does not want to stop.
	while (!s)
	{
		std::string job = DatabaseRequests::getNextJob(env);

		if (errno != 0)
		{
			print::warn("Unable to retrieve new job.", __FILE__, __LINE__);
		}

		std::vector<std::string> splitted = Utils::split(job, INNER_DELIMITER);
		if (splitted.size() < 1)
		{
			print::warn("Incorrect database response.", __FILE__, __LINE__);
		}
		if (splitted[0] == "Spider")
		{
			print::log("New job: Download and parse " + splitted[2], __FILE__, __LINE__);
			if (splitted.size() < 5 || splitted[2] == "")
			{
				// We cannot signal this error to the database, since there is no guarantee that the
				// jobid and jobTime are correct or even present (the data from the database is malformed).
				print::warn("Unexpected job data received from database.", __FILE__, __LINE__);
			}
			stopped = false;
			long long startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
									  std::chrono::system_clock::now().time_since_epoch())
									  .count();
			new std::thread(&Start::handleTimeout, splitted[4], std::ref(startTime));
			versionProcessing(splitted, &startTime, flags, DOWNLOAD_LOCATION + std::to_string(thread), env);
			stopped = true;
			std::this_thread::sleep_for(std::chrono::seconds(5));
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
			print::warn("Incorrect database response.", __FILE__, __LINE__);
		}

		// Check if we need to stop.
		mtx.lock();
		s = stop;
		mtx.unlock();
	}
}

void Start::handleTimeout(const std::string timeoutString, long long &startTime)
{
	long long timeout = std::stoll(timeoutString);
	while (!stopped)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		long long currTime =
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
				.count();
		if (startTime + timeout < currTime)
		{
			print::warn("Job timed out.", __FILE__, __LINE__);
			stopped = true;
		}
	}
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

void Start::versionProcessing(std::vector<std::string> &splitted, long long *startTime, Flags flags, std::string downloadLocation, EnvironmentDTO *env)
{
	if (splitted.size() < 5 || splitted[2] == "")
	{
		// We cannot signal this error to the database, since there is no guarantee that the
		// jobid and jobTime are correct or even present (the data from the database is malformed).
		print::warn("Unexpected job data received from database.", __FILE__, __LINE__);
		return;
	}

	std::string jobid       = splitted[1];
	flags.mandatoryArgument = splitted[2];

	std::string jobTime = splitted[3];

	// Process and upload project.
	Command::uploadProject(flags, jobid, jobTime, startTime, env, downloadLocation);

	if (errno == 0)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::success, "Success.", env);
	}
	else if (errno != HANDLED_ERRNO)
	{
		DatabaseRequests::finishJob(jobid, jobTime, FinishReason::unknown,
									"Unknown error: " + std::to_string(errno) + ".", env);
		print::warn("Unexpected error occured with errno " + std::to_string(errno) + ".", __FILE__, __LINE__);
	}
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
	checkProject(flags, env);
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

	// Process and upload project.
	if (flags.flag_lines != "" || flags.flag_vulnCode != "" || flags.flag_projectCommit != "")
	{
		if (flags.flag_lines != "" && flags.flag_vulnCode != "" && flags.flag_projectCommit != "")
		{
			std::map<std::string, std::vector<int>> fileLines;
			std::vector<std::string> files = Utils::split(flags.flag_lines, '?');
			for (std::string file : files)
			{
				std::vector<std::string> splitted = Utils::split(file, ':');
				std::vector<std::string> lines = Utils::split(splitted[1], ',');
				std::vector<int> intLines;
				for (std::string line : lines)
				{
					intLines.push_back(std::stoi(line));
				}
				fileLines[splitted[0]] = intLines;
			}
			Command::uploadPartialProject(flags, flags.flag_projectCommit, fileLines, flags.flag_vulnCode, env);
		}
		else
		{
			print::warn("Lines, vulnerability code or project commit was specified, but not all of them. Pleas specify all or none.", __FILE__, __LINE__);
		}
	}
	else
	{
		std::string jobTime = "";
		Command::uploadProject(flags, "", jobTime, nullptr, env);
	}
	if (errno == 0)
	{
		this->logPostExecutionMessage(flags.mandatoryArgument, __FILE__, __LINE__);
	}
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

	checkProject(flags, env);

	Upload::logPreExecutionMessage(url, __FILE__, __LINE__);

	std::string jobTime = "";
	Command::uploadProject(flags, "", jobTime, nullptr, env);

	Upload::logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion CheckUpload
