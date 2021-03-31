#pragma once

#include "Utils.h"

class Flags
{
public:
	std::string mandatoryArgument;
	int flag_cpu;
	int flag_ram;
	std::string flag_output;
	bool flag_save;
	utils::VerbosityLevel flag_verbose;

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

