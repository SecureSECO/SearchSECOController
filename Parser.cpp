/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Parser.h"
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "Print.h"

// Parser constants

std::map<std::string, std::string> FlagParser::shorthandFlagToLong =
{
	{"v", "version"},
	{"V", "verbose"},
	{"c", "cpu"},
	{"r", "ram"},
	{"o", "output"},
	{"s", "save"}
};


std::map<std::string, std::string> FlagParser::parse(std::string configPath, std::string sourcePath, std::string command, std::string mandatoryArguments, std::map<std::string, std::string> optionalArguments)
{
	mapShortFlagToLong(optionalArguments);

	// handle commands that do not take any flags
	if (optionalArguments.count("version") != 0)
	{
		return { 
			{"command", "version"} 
		};
	}
	else if (optionalArguments.count("update") != 0)
	{
		return { 
			{ "command", "update" },
			{ "version", mandatoryArguments},
		};
	}
	else if (optionalArguments.count("help") != 0)
	{
		return { 
			{"command", "help"},
			{"with", command}
		};
	}

	// get default hardcoded dict
	std::map<std::string, std::string> flagArgs = FlagParser::getDefaultFlagsForCommand(sourcePath, command, mandatoryArguments);

	// apply user defaults 
	// TODO we need something to ensure base arguments are not overwritten by the file, maybe remove them from valid options if possible
	parseFile(flagArgs, configPath);

	// apply command values
	parseFlags(flagArgs, optionalArguments);

	// return result
	return flagArgs;
}

void FlagParser::mapShortFlagToLong(std::map<std::string, std::string> &flargs)
{
	std::map<std::string, std::string>::iterator it;

	for (it = flargs.begin(); it != flargs.end(); ++it)
	{
		std::string key = it->first;
		if (utils::isShortHandFlag(key))
		{
			std::map<std::string, std::string>::iterator lookup = flargs.find(key);
			if (lookup != flargs.end())
			{
				std::string value = lookup->second;
				flargs.erase(lookup);

				flargs[FlagParser::shorthandFlagToLong[key]] = value;
			}
		}
	}
}

std::map<std::string, std::string> FlagParser::getDefaultFlagsForCommand(std::string sourcePath, std::string command, std::string mandatoryArguments)
{
	if (command == "start")
	{
		std::vector<std::string> temp = utils::split(mandatoryArguments, ' ');
		if (temp.size() != 1)
		{
			error::err_insufficient_arguments(command, 1, temp.size());
		}
		std::string storage = temp[0]; // TODO make sure this is a number >= 50
		
		std::string location = sourcePath; //  TODO remove searchseco.exe at the end
		return
		{
			{"command", command},
			{"storage", storage}, // storage in GB
			{"location", location}, 
			{"cores", ""},
			{"ram", ""},
			{"verbose", "4"},
		};
	}
	else if(command == "check" || command == "checkupload")
	{
		std::vector<std::string> temp = utils::split(mandatoryArguments, ' ');
		if (temp.size() != 1)
		{
			error::err_insufficient_arguments(command, 1, temp.size());
		}
		std::string url = temp[0]; //  TODO check if valid url?
		return
		{
			{"command", command},
			{"url", url},
			{"report", "console"},
			{"save", "false"},
			{"verbose", "4"},
		};
	}
	else if (command == "upload")
	{
		std::vector<std::string> temp = utils::split(mandatoryArguments, ' ');
		if (temp.size() != 1)
		{
			error::err_insufficient_arguments(command, 1, temp.size());
		}
		std::string url = temp[0]; //  TODO check if valid url?
		return
		{
			{"command", command},
			{"url", url},
			{"save", "false"},
			{"verbose", "4"},
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

void FlagParser::parseFlags(std::map<std::string, std::string>& flagArgs, std::map<std::string, std::string> optionalArguments)
{
	// TODO implement:
	//foreach (key in optionals) if (value specified) sanitize(value) otherwise default
	/*for (int i = start; i < argc; i++)
	{
		std::string flag = args[i];
		sanitize(flagArgs, flag, args[++i], false);
	}*/
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


