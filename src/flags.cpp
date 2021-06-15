/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "flags.h"
#include "print.h"
#include "utils.h"

// External includes
#include <fstream>
#include <thread>


#define RAM_DEFAULT 8 // TODO What amount of ram makes sense

std::map<std::string, std::string> Flags::shorthandFlagToLong =
{
	{"h", "help"},
	{"v", "version"},
	{"V", "verbose"},
	{"c", "cpu"},
	{"b", "branch"}
};

Flags::Flags()
{
	this->mandatoryArgument = "";
	this->flag_cpu = std::max(1u, std::thread::hardware_concurrency() / 2);
	this->flag_verbose = loguru::Verbosity_INFO;

	this->flag_help = false;
	this->flag_version = false;
	this->flag_branch = "master";

	this->flag_github_user = "";
	this->flag_github_token = "";
}

void Flags::mapShortFlagToLong(std::map<std::string, std::string>& flargs)
{
	std::map<std::string, std::string> temp = {}; 
	std::map<std::string, std::string>::iterator it;

	for (it = flargs.begin(); it != flargs.end(); ++it)
	{
		std::string key = it->first;
		std::string value;

		std::map<std::string, std::string>::iterator lookup = flargs.find(key);
		if (lookup != flargs.end())
		{
			value = lookup->second;
		}
		else
		{
			continue;
		}

		if (Flags::isShortHandFlag(key))
		{
			std::string fullLength = Flags::shorthandFlagToLong[key];

			print::debug(print::quote("-" + key) + " -> " + print::quote("--" + fullLength), __FILE__, __LINE__);
			temp[fullLength] = value;
		}
		else
		{
			temp[key] = value;   
		}
	}
	flargs = temp;
}

bool Flags::isFlag(std::string flag)
{
	return Flags::isShortHandFlag(flag) || Flags::isLongFlag(flag);
}

bool Flags::isShortHandFlag(std::string flag)
{
	return Flags::shorthandFlagToLong.count(flag) != 0;
}

bool Flags::isLongFlag(std::string flag)
{
	std::map<std::string, std::string> m = Flags::shorthandFlagToLong;
	std::map<std::string, std::string>::iterator it;

	for (it = m.begin(); it != m.end(); ++it)
	{
		if (it->second == flag)
		{
			return true;
		}
	}

	return false;
}

std::map<std::string, std::string> Flags::parseConfig(std::string configPath)
{
	std::ifstream configFile(configPath);

	std::string line;
	std::vector<std::string> flagArg;
	std::string flag;
	std::string arg;

	std::map<std::string, std::string> configFlagArgs = {};
	auto n = 0;

	while (std::getline(configFile, line))
	{
		flagArg = utils::split(line, ':');
		flag = utils::trimWhiteSpaces(flagArg[0]);
		arg = utils::trimWhiteSpaces(flagArg[1]);

		configFlagArgs[flag] = arg;
		++n;
	}

	configFile.close();

	if (n == 0)
	{
		print::warn("No or empty configuration file was found at " + print::quote(configPath), __FILE__, __LINE__);
	}

	return configFlagArgs;
}
