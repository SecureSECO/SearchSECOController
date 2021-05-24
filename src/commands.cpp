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


#define DOWNLOAD_LOCATION "spiderDownloads"
#define CONTROLLER_THREAD_NAME "controller"

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

void Start::logPreExecutionMessage(int fCPU, int fRAM, const char* file, int line)
{
	auto msg = "Starting a worker node with "
		+ std::to_string(fCPU) + " cpu cores and "
		+ std::to_string(fRAM) + "GB RAM";
	print::log(msg, file, line);
}

void Start::logPostExecutionMessage(const char* file, int line)
{
	loguru::set_thread_name(CONTROLLER_THREAD_NAME);
	print::log("Successfully terminated the worker node", file, line);
}

void Start::execute(Flags flags)
{
	int
		fCPU = flags.flag_cpu,
		fRAM = flags.flag_ram;

	Start::logPreExecutionMessage(fCPU, fRAM, __FILE__, __LINE__);

	error::errNotImplemented("start", __FILE__, __LINE__);

	Start::logPostExecutionMessage(__FILE__, __LINE__);
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
	loguru::set_thread_name(CONTROLLER_THREAD_NAME);
	print::log("Successfully checked" + Check::partialLogMessage(url), file, line);
}

void Check::execute(Flags flags)
{
	auto url = flags.mandatoryArgument;

	this->logPreExecutionMessage(url, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);

	// Calling the function that will print all the matches for us.
	printMatches::printHashMatches(hashes, DatabaseRequests::findMatches(hashes), authorData);
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
	loguru::set_thread_name(CONTROLLER_THREAD_NAME);
	print::log("Successfully uploaded" + Upload::partialLogMessage(url), file, line);
}

void Upload::execute(Flags flags)
{
	auto url = flags.mandatoryArgument;

	this->logPreExecutionMessage(url, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);

	// Uploading the hashes.
	ProjectMetaData meta = moduleFacades::getProjectMetadata(url);
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
	auto url = flags.mandatoryArgument;

	Check::logPreExecutionMessage(url, __FILE__, __LINE__);

	AuthorData authorData = moduleFacades::downloadRepository(url, flags, DOWNLOAD_LOCATION);
	std::vector<HashData> hashes = moduleFacades::parseRepository(DOWNLOAD_LOCATION, flags);

	Check::logPostExecutionMessage(url, __FILE__, __LINE__);


	Upload::logPreExecutionMessage(url, __FILE__, __LINE__);

	ProjectMetaData metaData = moduleFacades::getProjectMetadata(url);

	printMatches::printHashMatches(hashes, DatabaseRequests::checkUploadHashes(hashes, metaData, authorData), authorData);

	Upload::logPostExecutionMessage(url, __FILE__, __LINE__);
}

#pragma endregion CheckUpload

#pragma region Update

Update::Update()
{
	this->helpMessageText = R"(
	update: Updates the program.
		Optionals:
			The version you want to update to. If no version is specified, the most recent version will be used.)";
}

void Update::logPreExecutionMessage(std::string targetVersion, const char* file, int line)
{
	print::log("Attempting to update searchseco to version " + targetVersion, file, line);
}

void Update::logPostExecutionMessage(const char* file, int line)
{
	loguru::set_thread_name(CONTROLLER_THREAD_NAME);
	print::log("Succesfully updated searchseco and its submodules", file, line);
}

void Update::execute(Flags flags)
{
	auto target = flags.mandatoryArgument;

	Update::logPreExecutionMessage(target, __FILE__, __LINE__);

	error::errNotImplemented("update", __FILE__, __LINE__);

	Update::logPostExecutionMessage(__FILE__, __LINE__);
}

#pragma endregion Update
