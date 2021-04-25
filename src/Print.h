/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

#include <string>
#include <vector>

#include "../parser/Parser/HashData.h"

#include "Utils.h"

namespace print
{
	/// <summary>
	/// Logs a debug message.
	/// </summary>
	void debug(std::string msg, const char* file, int line);

	/// <summary>
	/// Log a message to stdout, and log it to a file.
	/// </summary>
	void log(std::string msg, const char* file, int line);

	/// <summary>
	/// Print a warning message to stdout, and log to a file.
	/// </summary>
	void warn(std::string msg, const char* file, int line);

	/// <summary>
	/// Prints a given string to the console, followed by a newline character.
	/// </summary>
	/// <param name="str">The string to output.</param>
	void printline(std::string str);

	/// <summary>
	/// Returns a string of n tabs.
	/// </summary>
	std::string tab(int n = 1);

	/// <summary>
	/// Encapsulates a string with a single character; so once before the string, and once behind it.
	/// </summary>
	/// <param name="str">The base string to be encapsulated.</param>
	/// <param name="c">The character to encapsulate with.</param>
	/// <returns>A string which is c + str + c.</returns>
	std::string encapsulate(std::string str, char c);

	/// <summary>
	/// Puts a string in quotation marks.
	/// </summary>
	/// <param name="str">The string to put inside quotation marks.</param>
	/// <returns>A quoted string, e.g. "string".</returns>
	std::string quote(std::string str);

	/// <summary>
	/// Returns either the singular or the plural form of the string supplied, depending on n.
	/// </summary>
	std::string plural(std::string singular, int n);

	/// <summary>
	/// Prints the Controller version, and the versions of the subsystems to the console.
	/// </summary>
	void version_full();

	/// <summary>
	/// Prints information about the hash matches that were found in the database.
	/// </summary>
	void printHashMatches(std::vector<HashData> hashes, std::string databaseOutput);
}

namespace error
{
	/// <summary>
	/// Throws the "Insufficient Arguments" error for a command.
	/// </summary>
	/// <param name="command">Name of the command which has insufficient arguments.</param>
	void err_cmd_incorrect_arguments(std::string command, int expected, int received, const char* file, int line);

	/// <summary>
	/// Throws the "Insufficient Arguments" error for a flag.
	/// </summary>
	/// <param name="command">Name of the flag which has insufficient arguments.</param>
	void err_flag_incorrect_arguments(std::string flag, int expected, int received, const char* file, int line);

	/// <summary>
	/// Throws the "Flag Does Not Exist" error.
	/// </summary>
	/// <param name="flag">The flag that does not exist.</param>
	/// <param name="from_config">Whether this flag appeared in the config file (true) or was entered in the command line (false).</param>
	void err_flag_not_exist(std::string flag, bool from_config, const char* file, int line);

	/// <summary>
	/// Throws the "Invalid Argument" error for flags.
	/// </summary>
	/// <param name="flag">The flag that had an invalid argument.</param>
	/// <param name="arg">The (invalid) argument.</param>
	/// <param name="from_config">Whether this flag appeared in the config file (true) or was entered in the command line (false).</param>
	void err_flag_invalid_arg(std::string flag, std::string arg, bool from_config, const char* file, int line);

	/// <summary>
	/// Throws the "Command Not Found" error.
	/// </summary>
	void err_cmd_not_found(const char* file, int line);

	/// <summary>
	/// Throws the "Command Does Not Exist" error.
	/// </summary>
	/// <param name="command">The command that does not exist.</param>
	void err_cmd_not_exist(std::string command, const char* file, int line);

	/// <summary>
	/// Throws the "Not Implemented" error.
	/// </summary>
	void err_not_implemented(std::string message, const char* file, int line);
};
