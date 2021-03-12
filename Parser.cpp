#include "Parser.h"
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "Print.h"

#define CONTROLLER_VERSION "0.0.1"

std::map<std::string, std::string> Parser::parse(std::string path, std::string* args, int argc)
{
	if (argc <= 1)
	{
		error::err_insufficient_arguments("searchSECO");
	}
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
	int currentArg = 1;
	// translate short flags to full words and remove dashes
	for (int i = currentArg; i < argc; i++)
	{
		args[i] = utils::trim(args[i], "-");
		if (shortToLong.count(args[i]) != 0)
		{
			args[i] = shortToLong[args[i]];
		}
	}
	// check if version is asked
	if (args[currentArg] == "version")
	{
		// Do something else
		return{{ "command", "version" }};
	}
	else if (args[currentArg] == "update")
	{
		// Update
		return{{ "command", "update" }};
	}

	// get default hardcoded dict
	std::map<std::string, std::string> flagArgs = getDefaultFlagsForCommand(args[currentArg++]);

	if (flagArgs.count("argument") != 0)
	{
		if (argc <= currentArg)
		{
			error::err_insufficient_arguments(args[currentArg - 1]);
		}
		flagArgs["argument"] = args[currentArg++];
	}




	// apply user defaults
	parseFile(flagArgs, path);

	// apply command values
	parseFlags(flagArgs, args, argc, currentArg);


	// return result
	return flagArgs;
}

std::map<std::string, std::string> Parser::getDefaultFlagsForCommand(std::string command)
{
	if (command == "start")
	{
		return
		{
			{"command", command},
			{"verbose", "4"},
			{"storage", ""}, // in GB
			{"cores", ""},
			{"location", ""},
		};
	}
	else if(command == "check" || command == "checkupload")
	{
		return
		{
			{"command", command},
			{"verbose", "4"},
			{"report", "console"},
			{"save", "false"},
			{"argument", ""},
		};
	}
	else if (command == "upload")
	{
		return
		{
			{"command", command},
			{"verbose", "4"},
			{"save", "false"},
			{"argument", ""},
		};
	}
	error::err_invalid_command(command);
	return std::map<std::string, std::string>();
}

void Parser::parseFile(std::map<std::string, std::string>& flagArgs, std::string path)
{
	std::ifstream configFile(path);

	std::string line;
	std::string* flagArg;
	std::string flag;
	std::string arg;

	while (std::getline(configFile, line))
	{
		flagArg = utils::split(line, ':');
		flag = utils::trimWhiteSpaces(flagArg[0]);
		arg = utils::trimWhiteSpaces(flagArg[1]);
		delete[] flagArg;

		sanitize(flagArgs, flag, arg, true);
	}

	configFile.close();
}

void Parser::parseFlags(std::map<std::string, std::string>& flagArgs, std::string* args, int argc, int start)
{

	for (int i = start; i < argc; i++)
	{
		std::string flag = args[i];
		sanitize(flagArgs, flag, args[++i], false);
	}
}

void Parser::sanitize(std::map<std::string, std::string>& flagArgs, std::string flag, std::string argument, bool fromFile)
{
	// see if the flag exists
	if (flagArgs.count(flag) != 1)
	{
		if (!fromFile)
		{
			error::err_flag_not_exist(flag, fromFile);
		}
		return;
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


