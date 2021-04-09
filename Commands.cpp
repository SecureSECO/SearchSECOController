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

// general function.

void Commands::execute(std::string s, std::map<std::string, std::string> flags) 
{
	perform[s](flags);
}

bool Commands::isCommand(std::string s) 
{
	return perform.count(s) >= 1;
}

// Commands.

void Commands::start(std::map<std::string, std::string> flags) 
{
	// Depends: crawler, spider, db, distribution.
	error::err_not_implemented("start");
}

void Commands::check(std::map<std::string, std::string> flags)
{
	// Depends: spider, db.
	std::string tempLocation = "spiderDownloads\\";
	tempLocation = Commands::downloadRepository(flags["argument"], flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(tempLocation, flags);
	// Calling the function that will print all the matches for us.
	print::printHashMatches(hashes, DatabaseRequests::findMatches(hashes));
	//TODO: delete temp folder.
}

void Commands::upload(std::map<std::string, std::string> flags)
{
	// Depends: spider, db.
	std::string tempLocation = "spiderDownloads\\";
	std::string location = Commands::downloadRepository(flags["argument"], flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(location, flags);

	// Uploading the hashes.
	ProjectMetaData meta = utils::getProjectMetaDataFromFile(tempLocation + "project_data.meta");
	print::printline(DatabaseRequests::uploadHashes(hashes, meta));
}

void Commands::checkupload(std::map<std::string, std::string> flags)
{
	// Depends: spider, db.
	std::string tempLocation = "spiderDownloads\\";
	std::string location = Commands::downloadRepository(flags["argument"], flags, tempLocation);
	std::vector<HashData> hashes = Commands::parseRepository(location, flags);

	ProjectMetaData metaData = utils::getProjectMetaDataFromFile(tempLocation + "project_data.meta");
	// Uploading the hashes.
	print::printHashMatches(hashes, DatabaseRequests::checkUploadHashes(hashes, metaData));
}

void Commands::update(std::map<std::string, std::string> flags)
{
	// Depends: a lot.
	error::err_not_implemented("update");
}

void Commands::version(std::map<std::string, std::string> flags)
{
	print::version_full();
}

void Commands::help(std::map<std::string, std::string> flags)
{
	if (flags["argument"] == "" && helpMessagesCommands.count(flags["argument"]) == 0)
	{
		print::printline("\nCommands: \n");
		for (std::string command : commandNames)
		{
			print::printline(helpMessagesCommands[command]);
			print::printline("");
		}
	}
	else
	{
		print::printline(helpMessagesCommands[flags["argument"]]);
	}
	print::printline("\nCommon flags: \n");
	for (std::string commonFlag : commonFlagNames)
	{
		print::printline(helpMessagesCommonFlags[commonFlag]);
		print::printline("");
	}
}

// Helpers.

std::string Commands::downloadRepository(std::string repository, std::map<std::string, std::string> flags, std::string downloadPath)
{
	return RunSpider::runSpider(repository);
}

std::vector<HashData> Commands::parseRepository(std::string repository, std::map<std::string, std::string> flags)
{
	// Set default value.
	int cores = -1;
	// Try to parse it.
	if (flags["cores"] != "")
	{
		cores = std::stoi(flags["cores"]) - 1;
	}
	
	return Parser::parse(repository, cores);
}

// Init dict.
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

std::map<std::string, std::string> Commands::helpMessagesCommands =
{
	{"start", R"(start: Starts a workernode.
	Arguments:
		Storage size (in GB)
		Location where the local database is stored.
	Optionals: 
		-c --cpu: Number of threads used (minimum 2, default half)
		-r --ram: RAM cap (in GB) (default no cap))"},
	{"check", R"(check: Takes a github url, downloads it and parses it. After that it will check for matches with the database.  
	Arguments:
		Url to a github repository.
	Optionals:
		-o --output: Console to print to the console, else you can give a file path.  
		-s --save: Save the parser results for later use.)"},
	{"upload", R"(upload: Takes a github url, downloads it and parses it. Results are send to the database.
	Arguments:
		Url to a github repository.  
	Optionals:
		-s --save: Save the parser results for later use.)"},
	{"checkupload", R"(checkupload: Takes a github url, and does both the check and upload.
	Arguments:
		Url to a github repository.
	Optionals:
		-o --output: Console to print to the console, else you can give a file path.  
		-s --save: Save the parser results for later use.)"},
	{"update", R"(update: Updates the program.
	Optionals:
		The version you want to update to. If no version is specified, the most recent version will be used.)"},
};

std::vector<std::string> Commands::commandNames =
{
	"start",
	"check",
	"upload",
	"checkupload",
	"update",
};

std::map<std::string, std::string> Commands::helpMessagesCommonFlags =
{
	{"verbose", R"(-V --verbose: Sets the verbosity level of the console output. Can be used in combination with any command.
	Arguments:
		The verbosity level. Allowed values are:
			1: Nothing 
			2: Only errors
			3: Warning and errors
			4: Everything (default value))"},
	{"version", R"(-v --version: Displays the version of the SearchSECO system, and the version of each of its subsystems.)"},
	{"help", "-h --help: Displays the help message. Can be used in conjunction with other flags to specify the message."},
};

std::vector<std::string> Commands::commonFlagNames =
{
	"verbose",
	"version",
	"help",
};