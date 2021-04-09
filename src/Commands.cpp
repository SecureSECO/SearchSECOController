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

// Commands

void Commands::start(Flags flags)
{
	// depends: crawler, spider, db, distribution
	error::err_not_implemented("start");
}

void Commands::check(Flags flags)
{
	// depends: spider, db
	std::string tempLocation = "spiderDownloads";
	Commands::downloadRepository(flags.mandatoryArgument, flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(tempLocation, flags);
	// temporary printing of all the hashes
	for (int i = 0; i < hashes.size(); i++)
	{
		print::printline(hashes[i].hash);
	}
	//TODO: delete temp folder
}

void Commands::upload(Flags flags)
{
	// depends: spider, db
	error::err_not_implemented("upload");
}

void Commands::checkupload(Flags flags)
{
	// depends: spider, db
	error::err_not_implemented("checkupload");
}

void Commands::update(Flags flags)
{
	// depends: a lot
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

// helpers

void Commands::downloadRepository(std::string repository, Flags flags, std::string downloadPath)
{
	RunSpider::runSpider(repository);
}

std::vector<HashData> Commands::parseRepository(std::string repository, Flags flags)
{
	return Parser::parse(repository, flags.flag_cpu);
}

// init dict
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
