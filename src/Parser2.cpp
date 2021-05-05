/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Parser2.h"
#include "Print.h"
#include <fstream>
#include "Utils.h"


std::map<std::string, std::string> FlagParser::parseConfig(std::string configPath)
{
	std::ifstream configFile(configPath);

	std::string line;
	std::vector<std::string> flagArg;
	std::string flag;
	std::string arg;

	std::map<std::string, std::string> configFlagArgs = {};

	while (std::getline(configFile, line))
	{
		flagArg = utils::split(line, ':');
		flag = utils::trimWhiteSpaces(flagArg[0]);
		arg = utils::trimWhiteSpaces(flagArg[1]);

		configFlagArgs[flag] = arg;
	}

	configFile.close();

	return configFlagArgs;
}

