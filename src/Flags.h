/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/


#pragma once

#include "Utils.h"

#include "loguru/loguru.hpp"

class Flags
{
public:
	std::string mandatoryArgument;
	int flag_cpu;
	int flag_ram;
	std::string flag_output;
	bool flag_save;
	loguru::Verbosity flag_verbose;

	bool flag_help;
	bool flag_version;

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
	/// <param name="flag"></param>
	/// <returns></returns>
	static bool isLongFlag(std::string flag);
};

