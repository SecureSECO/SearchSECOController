#include "Parser.h"
#include <fstream>
#include <iostream>
#include "Utils.h"

#define CONTROLLER_VERSION "0.0.1"

std::map<std::string, std::string> Parser::parse(std::string path, std::string* args, int argc)
{
	std::map<std::string, std::string> shortToLong =
	{
		{"v", "version"},
		{"V", "verbose"},
		{"S", "storage"},
		{"c", "cores"},
		{"l", "location"},
		{"r", "report"},
		{"s", "save"}
	};

	// get default hardcoded dict
	std::map<std::string, std::string> flagArgs =
	{
		{"version", CONTROLLER_VERSION},
		{"verbose", "4"},
		{"storage", ""}, // in GB
		{"cores", ""},
		{"location", ""},
		{"report", "console"},
		{"save", "false"},
	};

	// translate short flags to full words and remove dashes

	// apply user defaults
	parseFile(flagArgs, path);

	// apply command values

	// return result
	return flagArgs;
}

void Parser::parseFile(std::map<std::string, std::string>& flagArgs, std::string path) 
{
	std::ifstream configFile(path);
	
	std::string line;
	std::string* flagArg;
	std::string flag;
	std::string arg;
	
	while(std::getline(configFile, line))
	{
		flagArg = Utils::split(line, ':');
		flag = Utils::trimWhiteSpaces(flagArg[0]);
		arg = Utils::trimWhiteSpaces(flagArg[1]);
		delete[] flagArg;
		
		flagArgs[flag] = arg;
	}

	configFile.close();
}
