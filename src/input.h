/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/


#pragma once

#include <string>
#include <map>
#include <climits>

#include "Utils.h"
#include "Flags.h"

class Input
{
public:

	/// <summary>
	/// Creates the Input object, which will parse the command line arguments.
	/// </summary>
	Input(int argc, char* argv[]);

	/// <summary>
	/// Contains the command entered in the CLI by the user (e.g. "start", "upload" etc.).
	/// </summary>
	std::string command;

	/// <summary>
	/// Contains the full path to the executable.
	/// </summary>
	std::string executablePath;

	/// <summary>
	/// Contains the object representing the flags entered in the CLI by the user. This is filled
	///		with the default values for each flag by default, then potentially overwritten with 
	///		flags entered by the user.
	/// </summary>
	Flags flags;

private:

	/// <summary>
	/// Parses the CLI string. Performs regex to extract the command, flags, and executable path.
	/// </summary>
	void parseCliInput(int argc, char* argv[]);
	/// <summary>
	/// Extracts the path to the executable.
	/// </summary>
	void parseExecutablePath(std::string fullPath);
	/// <summary>
	/// Parses and extracts the optional flags entered by the user.
	/// </summary>
	/// <param name="flargs"></param>
	void parseOptionals(std::string flargs);

	/// <summary>
	/// Applies the default values for each potential flag. These will then be overwritten when the flag
	///		is supplied by the user.
	/// </summary>
	void applyDefaults();

	/// <summary>
	/// Checks if the arguments for each flag is valid and in the expected range.
	/// </summary>
	void sanitizeArguments();

	/// <summary>
	/// Sanitizes the argument of the (--cpu | -c) flag.
	/// </summary>
	void sanitizeCpuFlag(std::string arg, bool fromConfig);
	/// <summary>
	/// Sanitizes the argument of the (--ram | -r) flag.
	/// </summary>
	void sanitizeRamFlag(std::string arg, bool fromConfig);
	/// <summary>
	/// Sanitizes the argument of the (--output | -o) flag.
	/// </summary>
	void sanitizeOutputFlag(std::string arg, bool fromConfig);
	/// <summary>
	/// Sanitizes the argument of the (--save | -s) flag.
	/// </summary>
	void sanitizeSaveFlag(std::string arg, bool fromConfig);
	/// <summary>
	/// Sanitizes the argument of the (--verbose | -V) flag.
	/// </summary>
	void sanitizeVerboseFlag(std::string arg, bool fromConfig);
	/// <summary>
	/// Sanitizes the argument of the (--help | -h) flag.
	/// </summary>
	void sanitizeHelpFlag(std::string arg, bool fromConfig);
	/// <summary>
	/// Sanitizes the argument of the (--version | -v) flag.
	/// </summary>
	void sanitizeVersionFlag(std::string arg, bool fromConfig);

	/// <summary>
	/// Helper function to force a certain amount of arguments for a flag. Throws the correct error when 
	///		the amount is unexpected.
	/// </summary>
	void requireNArguments(int n, std::string flag, std::string argument);

	/// <summary>
	/// Helper function to validate an integer. Throws an error callback when the string cannot be parsed,
	///		and calls the success callback when it can. Also allows to force the parsed integer to be within 
	///		some bounds with the "min" and "max" parameters.
	/// </summary>
	/// <param name="argument">The argument to be parsed.</param>
	/// <param name="callback">The success callback.</param>
	/// <param name="error">The error callback.</param>
	/// <param name="min">The minimum value for the integer to be parsed.</param>
	/// <param name="max">The maximum value for the integer to be parsed.</param>
	template <typename callbackFunction, typename errorFunction>
	void validateInteger(
		std::string argument, 
		callbackFunction callback, 
		errorFunction error,
		int min = INT_MIN, 
		int max = INT_MAX);

	/// <summary>
	/// Helper container for the optional arguments. Is later translated to the Flags object.
	/// </summary>
	std::map<std::string, std::string> optionalArguments;
	/// <summary>
	/// Helper container for the source per flag. This can be "config" or "cli". This is used for throwing
	///		descriptive errors.
	/// </summary>
	std::map<std::string, std::string> flagSource;
};
