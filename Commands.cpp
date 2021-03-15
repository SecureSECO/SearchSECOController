/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Commands.h"
#include <iostream>
#include "Print.h"

// general function

void Commands::execute(std::string s, std::map<std::string, std::string> flags) 
{
	perform[s](flags);
}

bool Commands::isCommand(std::string s) 
{
	return perform.count(std::string(s)) >= 0;
}

// Commands

void start(std::map<std::string, std::string> flags) 
{
	std::cout << "started";
}

void check(std::map<std::string, std::string> flags)
{
	std::cout << "checking";
}

void upload(std::map<std::string, std::string> flags)
{
	std::cout << "uploading";
}

void checkupload(std::map<std::string, std::string> flags)
{
	std::cout << "checkuploading";
}

void update(std::map<std::string, std::string> flags)
{
	std::cout << "updating";
}

void version(std::map<std::string, std::string> flags)
{
	print::version_full();
}

// init dict
std::map<std::string, std::function<void(std::map<std::string, std::string>)>> Commands::perform = 
{
	{"start", start},
	{"check", check},
	{"upload", upload},
	{"checkupload", checkupload},
	{"update", update},
	{"update", version},
};
