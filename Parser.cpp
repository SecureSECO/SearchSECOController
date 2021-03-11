#include "Parser.h"
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "Print.h"

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
		flagArg = utils::split(line, ':');
		flag = utils::trimWhiteSpaces(flagArg[0]);
		arg = utils::trimWhiteSpaces(flagArg[1]);
		delete[] flagArg;

		sanitize(flagArgs, flag, arg, true);
	}

	configFile.close();
}

void Parser::sanitize(std::map<std::string, std::string>& flagArgs, std::string flag, std::string argument, bool fromFile)
{
	// see if the flag exists
	if (flagArgs.count(flag) != 1)
	{
		error::err_flag_not_exist(flag, fromFile);
	}

	// check if the flag is valid
	if (flag == "verbose")
	{
		std::string* levels = new std::string[]{ "1", "2", "3", "4", "5" };
		if (!utils::contains(levels, argument, 5))
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
			return;
		}
	}
	else if (flag == "storage")
	{
		// currently only checks if the argument is numerical, no cap is set
		if (!utils::is_number(argument))
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
			return;
		}
	}
	else if (flag == "cores")
	{
		// currently only checks if the argument is numerical, no cap is set
		if (!utils::is_number(argument) || argument == "0" || argument == "1")
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
			return;
		}
	}
	else if (flag == "location")
	{
		// TODO: do we want to check anything here?
	}
	else if (flag == "report")
	{
		std::string* allowed = new std::string[]{ "console", "file", "both" };
		if (!utils::contains(allowed, argument, 3))
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
			return;
		}
	}
	else if (flag == "save")
	{
		std::string* allowed = new std::string[]{ "t", "f", "true", "false" };
		if (!utils::contains(allowed, argument, 4))
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
			return;
		}

		// translate shorthands to full size
		if (argument == "t") argument = "true";
		else if (argument == "f") argument = "false";
	}

	flagArgs[flag] = argument;
}


