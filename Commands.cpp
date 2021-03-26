/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Commands.h"
#include <iostream>
#include "Print.h"
#include "spider/SearchSECOSpider/SearchSecoSpider.h"
#include "Utils.h"
#include "parser/Parser/Parser.h"
#include "DatabaseRequests.h"

// general function

void Commands::execute(std::string s, std::map<std::string, std::string> flags) 
{
	perform[s](flags);
}

bool Commands::isCommand(std::string s) 
{
	return perform.count(s) >= 1;
}

// Commands

void Commands::start(std::map<std::string, std::string> flags) 
{
	// depends: crawler, spider, db, distribution
	error::err_not_implemented("start");
}

void Commands::check(std::map<std::string, std::string> flags)
{
	// depends: spider, db
	std::string tempLocation = "spiderDownloads";
	Commands::downloadRepository(flags["argument"], flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(tempLocation, flags);
	// temporary printing of all the hashes
	print::printline(DatabaseRequests::findMatches(hashes));
	//TODO: delete temp folder
}

void Commands::upload(std::map<std::string, std::string> flags)
{
	// depends: spider, db
	std::string tempLocation = "spiderDownloads";
	Commands::downloadRepository(flags["argument"], flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(tempLocation, flags);
	// uploading the hashes
	print::printline(DatabaseRequests::uploadHashes(hashes));
}

void Commands::checkupload(std::map<std::string, std::string> flags)
{
	// depends: spider, db
	error::err_not_implemented("checkupload");
}

void Commands::update(std::map<std::string, std::string> flags)
{
	// depends: a lot
	error::err_not_implemented("update");
}

void Commands::version(std::map<std::string, std::string> flags)
{
	print::version_full();
}

void Commands::help(std::map<std::string, std::string> flags)
{
	print::printline("Help section is not yet implemented.");
}

// helpers

void Commands::downloadRepository(std::string repository, std::map<std::string, std::string> flags, std::string downloadPath)
{
	RunSpider::runSpider(repository);
}

std::vector<HashData> Commands::parseRepository(std::string repository, std::map<std::string, std::string> flags)
{
	// set default value
	int cores = -1;
	// try to parse it
	if (flags["cores"] != "")
	{
		cores = std::stoi(flags["cores"]) - 1;
	}
	
	return Parser::Parse(repository, cores);
}

// init dict
std::map<std::string, std::function<void(std::map<std::string, std::string>)>> Commands::perform =
{
	{"start", start},
	{"check", check},
	{"upload", upload},
	{"checkupload", checkupload},
	{"update", update},
	{"version", version},
	{"help", help}
};
