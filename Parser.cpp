/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "FlagParser.h"
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "Print.h"

std::map<std::string, std::string> FlagParser::parse(std::string path, std::string* args, int argc)
{
	// We need at least 2 arguments (first one is the path to the executable, second the command)
	if (argc <= 1)
	{
		error::err_insufficient_arguments("searchseco", 1, 0);
	}

	// translations from shorthand terms to what the mean
	std::map<std::string, std::string> shortToLong =
	{
		{"v", "version"},
		{"V", "verbose"},
		{"c", "cpu"},
		{"r", "ram"},
		{"o", "output"},
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
	// handle commands that do not take any flags
	if (args[currentArg] == "version")
	{
		return{ {"command", "version"} };
	}
	else if (args[currentArg] == "update")
	{
		return{{ "command", "update" }};
	}
	else if (args[currentArg] == "help")
	{
		return{ {"command", "help"} };
	}

	// get default hardcoded dict
	std::map<std::string, std::string> flagArgs = getDefaultFlagsForCommand(args[currentArg++]);

	// read direct argument for command, if it is needed
	if (flagArgs.count("argument") != 0)
	{
		if (argc <= currentArg)
		{
			error::err_insufficient_arguments(args[currentArg - 1], currentArg, argc); // TODO is this correct?
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

std::map<std::string, std::string> FlagParser::getDefaultFlagsForCommand(std::string command)
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
	error::err_cmd_not_exist(command);
	return std::map<std::string, std::string>();
}

void FlagParser::parseFile(std::map<std::string, std::string>& flagArgs, std::string path)
{
	std::ifstream configFile(path);

	std::string line;
	std::vector<std::string> flagArg;
	std::string flag;
	std::string arg;

	while (std::getline(configFile, line))
	{
		flagArg = utils::split(line, ':');
		flag = utils::trimWhiteSpaces(flagArg[0]);
		arg = utils::trimWhiteSpaces(flagArg[1]);

		sanitize(flagArgs, flag, arg, true);
	}

	configFile.close();
}

void FlagParser::parseFlags(std::map<std::string, std::string>& flagArgs, std::string* args, int argc, int start)
{
	for (int i = start; i < argc; i++)
	{
		std::string flag = args[i];
		sanitize(flagArgs, flag, args[++i], false);
	}
}

void FlagParser::sanitize(std::map<std::string, std::string>& flagArgs, std::string flag, std::string argument, bool fromFile)
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

	// check if the flag is valid, throw an error if not
	if (flag == "verbose")
	{
		std::string* levels = new std::string[]{ "1", "2", "3", "4", "5" };
		if (!utils::contains(levels, argument, 5))
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
		}
	}
	else if (flag == "storage")
	{
		// currently only checks if the argument is numerical, no cap is set
		if (!utils::isNumber(argument))
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
		}
	}
	else if (flag == "cores")
	{
		// currently only checks if the argument is numerical, no cap is set
		if (!utils::isNumber(argument) || argument == "0" || argument == "1")
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
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
		}
	}
	else if (flag == "save")
	{
		std::string* allowed = new std::string[]{ "t", "f", "true", "false" };
		if (!utils::contains(allowed, argument, 4))
		{
			error::err_flag_invalid_arg(flag, argument, fromFile);
		}

		// translate shorthands to full size
		if (argument == "t") argument = "true";
		else if (argument == "f") argument = "false";
	}

	// succesfully parsed the flag
	flagArgs[flag] = argument;
}


