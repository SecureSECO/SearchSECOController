/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "commands.h"
#include "databaseRequests.h"
#include "flags.h"
#include "moduleFacades.h"
#include "print.h"

// Parser includes
#include "Parser.h"

// Spider includes
#include "RunSpider.h"

#include <iostream>
#include <thread>


#define DOWNLOAD_LOCATION "spiderDownloads"

std::string Command::helpMessage()
{
	return helpMessageText;
}

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

Upload::Upload()
{
	this->helpMessageText = R"(
	upload: Takes a github url, downloads it and parses it. Results are sent to the database.
		Arguments:
			Url to a github repository.  
		Optionals:
			-s --save: Save the parser results for later use.)";
}

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

Update::Update()
{
	this->helpMessageText = R"(
	update: Updates the program.
		Optionals:
			The version you want to update to. If no version is specified, the most recent version will be used.)";
}

void Start::execute(Flags flags)
{
	auto msg = "Starting a worker node with "
		+ std::to_string(flags.flag_cpu) + " cpu cores and "
		+ std::to_string(flags.flag_ram) + "GB RAM";
	print::log(msg, __FILE__, __LINE__);

	bool s = stop;
	std::thread t(&Start::readCommandLine, this);
	while (!s)
	{
		std::string job = DatabaseRequests::getNextJob();

		std::vector<std::string> splitted = utils::split(job, '?');
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
		else if (splitted[0] == "No Job")
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
}

void Start::handleCrawlRequest(std::vector<std::string> &splitted, Flags flags)
{
	print::log("Start crawling", __FILE__, __LINE__);
	if (splitted.size() < 2)
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	CrawlData crawled = moduleFacades::crawlRepositories(std::stoi(splitted[1]));
	DatabaseRequests::addCrawledJobs(crawled);
}

void Start::handleSpiderRequest(std::vector<std::string> &splitted, Flags flags)
{
	print::log("Start parsing and uploading " + splitted[1], __FILE__, __LINE__);
	Upload upload = Upload();
	if (splitted.size() < 2)
	{
		error::errInvalidDatabaseAnswer(__FILE__, __LINE__);
	}
	flags.mandatoryArgument = splitted[1];
	ProjectMetaData meta = utils::getProjectMetadata(flags.mandatoryArgument);
	flags.flag_branch = meta.defaultBranch;
	AuthorData authorData = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
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

void Check::execute(Flags flags)
{
	auto msg = "Checking the code from the project at "
		+ flags.mandatoryArgument + " against the SearchSECO database";
	print::log(msg, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
	// Calling the function that will print all the matches for us.
	printMatches::printHashMatches(hashes, DatabaseRequests::findMatches(hashes), authorData);
	//TODO: delete temp folder.
}

void Upload::execute(Flags flags)
{
	auto msg = "Uploading the code from the project at "
		+ flags.mandatoryArgument + " to the SearchSECO database";
	print::log(msg, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);
	if (hashes.size() == 0)
	{
		return;
	}
	// Uploading the hashes.
	ProjectMetaData meta = utils::getProjectMetadata(flags.mandatoryArgument);
	print::printline(DatabaseRequests::uploadHashes(hashes, meta, authorData));
}

void CheckUpload::execute(Flags flags)
{
	auto msg = "Checking the code from the project at "
		+ flags.mandatoryArgument + " against the SearchSECO database";
	print::log(msg, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(flags.mandatoryArgument, flags, DOWNLOAD_LOCATION);
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);

	ProjectMetaData metaData = utils::getProjectMetadata(flags.mandatoryArgument);
	// Uploading the hashes.
	msg = "Uploading the code from the project at "
		+ flags.mandatoryArgument + " to the SearchSECO database";
	print::log(msg, __FILE__, __LINE__);

	printMatches::printHashMatches(hashes, DatabaseRequests::checkUploadHashes(hashes, metaData, authorData), authorData);
}

void Update::execute(Flags flags)
{
	auto msg = "Attempting to update searchseco to version "
		+ flags.mandatoryArgument;
	print::log(msg, __FILE__, __LINE__);

	error::errNotImplemented("update", __FILE__, __LINE__);
}