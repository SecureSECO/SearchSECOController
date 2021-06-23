/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "utils.h"

// External includes.
#include <map>
#include "loguru/loguru.hpp"
#include <string>


class Flags
{
public:
	std::string mandatoryArgument;
	int flag_cpu;
	std::string flag_branch;
	loguru::Verbosity flag_verbose;

	bool flag_help;
	bool flag_version;

	std::string flag_github_user;
	std::string flag_github_token;

	Flags();

	/// <summary>
	/// Maps the shorthand names of the flags to their longer versions. For consistency.
	/// </summary>
	static void mapShortFlagToLong(std::map<std::string, std::string>& flargs);

	static std::map<std::string, std::string> shorthandFlagToLong;

	/// <summary>
	/// Checks if a given flag is a valid flag.
	/// </summary>
	static bool isFlag(std::string flag);

	/// <summary>
	/// Checks if a given flag is represented as its shorthand name.
	/// </summary>
	static bool isShortHandFlag(std::string flag);

	/// <summary>
	/// Checks if a given flag is represented as its long name.
	/// </summary>
	static bool isLongFlag(std::string flag);

	/// <summary>
	/// Will read out the given config file and set the flags and arguments in the map given
	/// to the value found in the config file.
	/// </summary>
	/// <param name="flagArgs">The map with of which the values will be updated.</param>
	/// <param name="path">The path of the config file.</param>
	static std::map<std::string, std::string> parseConfig(std::string configPath);
};
