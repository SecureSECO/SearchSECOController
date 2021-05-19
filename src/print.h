/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes
#include "utils.h"

// Parser includes
#include "HashData.h"
#include "CodeBlock.h"

// External includes
#include <string>
#include <vector>


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
	void versionFull();

}

class printMatches
{
public:
	/// <summary>
	/// Prints information about the hash matches that were found in the database.
	/// </summary>
	static void printHashMatches(std::vector<HashData> hashes, std::string databaseOutput, AuthorData authordata);

private:
	static void printMatch(
		HashData hash,
		std::map<std::string, std::vector<std::string>>& receivedHashes,
		std::map<HashData, std::vector<std::string>>& authors, 
		std::map<std::string, int>& authorCopiedForm,
		std::map<std::string, int>& authorsCopied,
		std::map<std::string, std::vector<std::string>> &dbProjects,
		std::map<std::string, std::vector<std::string>> &authorIdToName
	);
	static void printSummary(std::map<std::string, int> authorCopiedForm, 
		std::map<std::string, int> authorsCopied, 
		int matches,
		std::map<std::string, std::vector<std::string>>& dbProjects,
		std::map<std::string, std::vector<std::string>>& authorIdToName,
		std::map<std::pair<std::string, std::string>, int> projects);

};

namespace error
{
	/// <summary>
	/// Throws the "Insufficient Arguments" error for a command.
	/// </summary>
	/// <param name="command">Name of the command which has insufficient arguments.</param>
	void errCmdIncorrectArguments(std::string command, int expected, int received, const char* file, int line);

	/// <summary>
	/// Throws the "Insufficient Arguments" error for a flag.
	/// </summary>
	/// <param name="command">Name of the flag which has insufficient arguments.</param>
	void errFlagIncorrectArguments(std::string flag, int expected, int received, const char* file, int line);

	/// <summary>
	/// Throws the "Flag Does Not Exist" error.
	/// </summary>
	/// <param name="flag">The flag that does not exist.</param>
	/// <param name="from_config">Whether this flag appeared in the config file (true) or was entered in the command line (false).</param>
	void errFlagNotExist(std::string flag, bool from_config, const char* file, int line);

	/// <summary>
	/// Throws the "Invalid Argument" error for flags.
	/// </summary>
	/// <param name="flag">The flag that had an invalid argument.</param>
	/// <param name="arg">The (invalid) argument.</param>
	/// <param name="from_config">Whether this flag appeared in the config file (true) or was entered in the command line (false).</param>
	void errFlagInvalidArg(std::string flag, std::string arg, bool from_config, const char* file, int line);

	/// <summary>
	/// Throws the "Command Not Found" error.
	/// </summary>
	void errCmdNotFound(const char* file, int line);

	/// <summary>
	/// Throws the "Command Does Not Exist" error.
	/// </summary>
	/// <param name="command">The command that does not exist.</param>
	void errCmdNotExist(std::string command, const char* file, int line);

	/// <summary>
	/// Throws when the user-entered call string could not be parsed (was syntactically incorrect).
	/// </summary>
	/// <param name="callstring">The call string in question.</param>
	void errParseCallSyntaxError(std::string callstring, const char* file, int line);

	/// <summary>
	/// Throws when a single-character flag was entered, preceded by double hyphens (as if it were a full-length
	///		flag).
	/// </summary>
	/// <param name="flag">The incorrect flag.</param>
	void errParseIncorrectShorthandFlag(std::string flag, const char* file, int line);

	/// <summary>
	/// Throws when a multi-character flag was entered, preceded by a single hyphen (as if it were a shorthand
	///		flag).
	/// </summary>
	/// <param name="flag">The incorrect flag.</param>
	void errParseIncorrectLonghandFlag(std::string flag, const char* file, int line);

	/// <summary>
	/// Throws when a flag was entered that could not be parsed (too many hyphens, for example).
	/// </summary>
	/// <param name="flag">The incorrect flag.</param>
	void errParseCouldNotParseFlag(std::string flag, const char* file, int line);

	/// <summary>
	/// Throws when the URL entered by the user was incorrect.
	/// </summary>
	/// <param name="url">The 'URL' in question.</param>
	void errInvalidUrl(std::string url, const char* file, int line);

	/// <summary>
	/// Throws the "Not Implemented" error.
	/// </summary>
	void errNotImplemented(std::string message, const char* file, int line);
};
