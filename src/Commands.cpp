/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Commands.h"
#include <iostream>
#include "Print.h"
#include "../spider/SearchSECOSpider/SearchSecoSpider.h"
#include "Utils.h"
#include "../parser/Parser/Parser.h"
#include "DatabaseRequests.h"

#include "Flags.h"

void Commands::execute(std::string command, Flags flags) 
{

	if (flags.flag_help) {
		help(flags);
	}
	else if (flags.flag_version) {
		version(flags);
	}
	else {
		perform[command](flags);
	}
}

bool Commands::isCommand(std::string s) 
{
	return perform.count(s) >= 1;
}

// Commands.

void Commands::start(Flags flags)
{
	// Depends: crawler, spider, db, distribution.
	error::err_not_implemented("start");
}

void Commands::check(Flags flags)
{
	std::string tempLocation = "spiderDownloads";
	Commands::downloadRepository(flags.mandatoryArgument, flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(tempLocation, flags);
	// Calling the function that will print all the matches for us.
	print::printHashMatches(hashes, DatabaseRequests::findMatches(hashes));
	//TODO: delete temp folder.
}

void Commands::upload(Flags flags)
{
	// Depends: spider, db.
	std::string tempLocation = "spiderDownloads\\";
	std::string location = Commands::downloadRepository(flags.mandatoryArgument, flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(location, flags);

	// Uploading the hashes.
	ProjectMetaData meta = utils::getProjectMetaDataFromFile(tempLocation + "project_data.meta");
	print::printline(DatabaseRequests::uploadHashes(hashes, meta));
}

void Commands::checkupload(Flags flags)
{
	// Depends: spider, db.
	std::string tempLocation = "spiderDownloads\\";
	std::string location = Commands::downloadRepository(flags.mandatoryArgument, flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(location, flags);

	ProjectMetaData metaData = utils::getProjectMetaDataFromFile(tempLocation + "project_data.meta");
	// Uploading the hashes.
	print::printHashMatches(hashes, DatabaseRequests::checkUploadHashes(hashes, metaData));
}

void Commands::update(Flags flags)
{
	// Depends: a lot.
	error::err_not_implemented("update");
}

void Commands::version(Flags flags)
{
	print::version_full();
}

void Commands::help(Flags flags)
{
	print::printline("Help section is not yet implemented.");
}

// Helpers.

std::string Commands::downloadRepository(std::string repository, Flags flags, std::string downloadPath)
{
	return RunSpider::runSpider(repository);
}

std::vector<HashData> Commands::parseRepository(std::string repository, Flags flags)
{
	return Parser::parse(repository, flags.flag_cpu);
}

std::map<std::string, std::function<void(Flags)>> Commands::perform =
{
	{"start", start},
	{"check", check},
	{"upload", upload},
	{"checkupload", checkupload},
	{"update", update},
	{"version", version},
	{"help", help}
};
