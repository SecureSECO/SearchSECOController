/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "dto.h"

// Parser includes.
#include "HashData.h"

// Spider includes.
#include "CodeBlock.h"

#include <set>


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
	/// Sets loguru to "silent mode", i.e. it will not log anything to stderr.
	/// </summary>
	void loguruSetSilent();

	/// <summary>
	/// Resets the thread name back to our name in loguru. Used when control is returned to us.
	/// </summary>
	void loguruResetThreadName();

	/// <summary>
	/// Prints a given string to the console, followed by a newline character.
	/// </summary>
	/// <param name="str">The string to output.</param>
	void printline(std::string str);

	/// <summary>
	/// Appends a given string to a file.
	/// </summary>
	void writelineToFile(std::string str, std::ofstream &file);

	/// <summary>
	/// Prints a line to stdout, and appends it to a file.
	/// </summary>
	void printAndWriteToFile(std::string str, std::ofstream &file);

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

class PrintMatches
{
public:
	/// <summary>
	/// Prints information about the hash matches that were found in the database.
	/// </summary>
	/// <param name="hashes">The hashes found locally.</param>
	/// <param name="databaseOutput">The matches found by the database.</param>
	/// <param name="authordata">The author data for the project we parsed locally.</param>
	static void printHashMatches(
		std::vector<HashData>& hashes, 
		std::string databaseOutput, 
		AuthorData &authordata, 
		EnvironmentDTO *env,
		std::string url,
		std::string projectID
	);

private:
	struct Method
	{
		std::string hash;
		std::string projectID;
		std::string startVersion;
		std::string startVersionHash;
		std::string endVersion;
		std::string endVersionHash;
		std::string name;
		std::string file;
		std::string lineNumber;
		std::string parserVersion;
		std::string vulnCode;
		int numberOfAuthors;
		std::vector<std::string> authors;
		std::string license;
	};

	/// <summary>
	/// Will parse the dbentries into the other parameters given.
	/// </summary>
	/// <param name="dbentries">The function input. This is what we got from the database.</param>
	/// <param name="receivedHashes">The hashes in the database output, stored in a map where
	/// the key is the hash, and the std::vector is the rest of the data given by the database.
	/// This data will be in the same order as the database gave it to us.</param>
	/// <param name="projects">The projects found in the database.
	/// The key for this map is a pair where the first is the project id,
	/// and the second is the project version.
	/// The value after is how often we came across this project.</param>
	/// <param name="dbAuthors">The authors we found in the database output.
	/// The key for this map is the author id,
	/// the value is how often we found this author.</param>
	static void parseDatabaseHashes(
		std::vector<std::string>& dbentries,
		std::map<std::string, std::vector<Method>>& receivedHashes,
		std::map<std::string, int> &projectMatches,
		std::set<std::pair<std::string, std::string>> &projectVersions,
		std::map<std::string, int> &dbAuthors);

	/// <summary>
	/// Will send database requests to retrieve the missing information.
	/// "projects" and "dbAuthors" are input here, 
	/// and "dbProjects" and "authorIdToName" are output.
	/// </summary>
	/// <param name="projects">Output from parseDatabasehHashes.</param>
	/// <param name="dbAuthors">Output from parseDatabasehHashes.</param>
	/// <param name="dbProjects">The key will be the id of the project,
	/// the value is a list of all the things the database gave us about the project,
	/// in the same order as we got it.</param>
	/// <param name="authorIdToName">The key will be the id of the author,
	/// the value is a list of all the things the database gave us about the author,
	/// in the same order as we got it.</param>
	static void getDatabaseAuthorAndProjectData(
		std::set<std::pair<std::string, std::string>> &projectVersions,
		std::map<std::string, int>& dbAuthors,
		std::map<std::string, std::vector<std::string>>& dbProjects,
		std::map<std::string, std::vector<std::string>>& authorIdToName, 
		EnvironmentDTO *env);

	/// <summary>
	/// Prints a match for a given hash.
	/// </summary>
	static void printMatch(std::vector<HashData> &hashes, std::vector<Method> dbEntries,
						   std::map<HashData, std::vector<std::string>> &authors, std::string projectID,
						   std::map<std::string, int> &authorCopiedForm, std::map<std::string, int> &authorsCopied,
						   std::vector<std::pair<HashData *, Method>> &vulnerabilities,
						   std::map<std::string, std::vector<std::string>> &dbProjects,
						   std::map<std::string, std::vector<std::string>> &authorIdToName, std::string &report);

	/// <summary>
	/// Prints a summary for all the matches found.
	/// </summary>
	static void printSummary(std::map<std::string, int> &authorCopiedForm, std::map<std::string, int> &authorsCopied,
							 std::vector<std::pair<HashData *, Method>> &vulnerabilities, int matches, int methods,
							 std::map<std::string, std::vector<std::string>> &dbProjects,
							 std::map<std::string, std::vector<std::string>> &authorIdToName,
							 std::map<std::string, int> &projects, std::ofstream &report);

	/// <summary>
	/// Sets up the file for the plaintext output report.
	/// </summary>
	/// <returns>The pointer to the filestream.</returns>
	static std::ofstream setupOutputReport(std::string url);

	static Method getMethod(std::vector<std::string> entry);	
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
	/// <param name="from_config">Whether this flag appeared in the config file (true) 
	/// or was entered in the command line (false).</param>
	void errFlagNotExist(std::string flag, bool from_config, const char* file, int line);

	/// <summary>
	/// Throws the "Invalid Argument" error for flags.
	/// </summary>
	/// <param name="flag">The flag that had an invalid argument.</param>
	/// <param name="arg">The (invalid) argument.</param>
	/// <param name="from_config">Whether this flag appeared in the config file (true) 
	/// or was entered in the command line (false).</param>
	void errFlagInvalidArg(std::string flag, std::string arg, bool from_config, const char* file, int line);

	/// <summary>
	/// Throws the "Missing Github authentication tokens" error.
	/// </summary>
	void errMissingGithubAuth(const char* file, int line);

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
	/// Signals a fatal failure occurred in the crawler submodule.
	/// </summary>
	void errSubmoduleFatalFailureCrawler(const char* file, int line);

	/// <summary>
	/// Signals a fatal failure occurred in the spider submodule.
	/// </summary>
	void errSubmoduleFatalFailureSpider(const char* file, int line);

	/// <summary>
	/// Signals a fatal failure occurred in the parser submodule.
	/// </summary>
	void errSubmoduleFatalFailureParser(const char* file, int line);

	/// <summary>
	/// Throws the error if the database API responds in an unexpected manner.
	/// </summary>
	void errNoEnvFile(const char* file, int line);

	/// <summary>
	/// Throws the error if the database API responds in an unexpected manner.
	/// </summary>
	void errNoIpsInEnvFile(const char* file, int line);

	/// <summary>
	/// Throws the "Not Implemented" error.
	/// </summary>
	void errNotImplemented(std::string message, const char* file, int line);

	/// <summary>
	/// Throws the "Invalid database response" error.
	/// </summary>
	void errInvalidDatabaseAnswer(const char* file, int line);

	/// <summary>
	/// Throws the error for a failing connection to the database.
	/// </summary>
	void errDBConnection(std::string message, const char* file, int line);

	/// <summary>
	/// Throws the error if the database API threw an error in the process of processing current request.
	/// </summary>
	void errDBBadRequest(std::string message, const char* file, int line);

	/// <summary>
	/// Throws the error if the database API threw an error that something went wrong server side.
	/// </summary>
	void errDBInternalError(std::string message, const char* file, int line);

	/// <summary>
	/// Throws the error if the database API responds in an unexpected manner.
	/// </summary>
	void errDBUnknownResponse(const char* file, int line);
};
