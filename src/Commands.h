/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

#include <map>
#include <string>
#include <functional>
#include <vector>

#include "Flags.h"

#include "../parser/Parser/HashData.h"

class Commands
{
public:
	/// <summary>
	/// Will execute the given command with the flags you give it.
	/// </summary>
	/// <param name="command">The command you want to execute.</param>
	/// <param name="flags">The flags with which you want to execute the command.</param>
	static void execute(std::string command, Flags flags);

	/// <summary>
	/// Checks if there is a command with the given name.
	/// </summary>
	/// <param name="command">The command you want to check if it exists.</param>
	static bool isCommand(std::string command);
private:
	/// <summary>
	/// Will call the spider to download a given repository.
	/// </summary>
	static AuthorData downloadRepository(std::string repository, Flags flags, std::string downloadPath);
	/// <summary>
	/// Will call the parser to parse the given repository.
	/// </summary>
	static std::vector<HashData> parseRepository(std::string repository, Flags flags);

	static std::map<std::string, std::function<void(Flags)>> perform;
	static std::map<std::string, std::string> helpMessagesCommands;
	static std::vector<std::string> commandNames;
	static std::map<std::string, std::string> helpMessagesCommonFlags;
	static std::vector<std::string> commonFlagNames;

	/// <summary>
	/// Starts the worker node.
	/// </summary>
	static void start(Flags flags);
	/// <summary>
	/// Checks matches with the database for the given repository.
	/// </summary>
	static void check(Flags flags);
	/// <summary>
	/// Uploads given repository.
	/// </summary>
	static void upload(Flags flags);
	/// <summary>
	/// Does both check and upload for a given repository.
	/// </summary>
	static void checkupload(Flags flags);
	/// <summary>
	/// Will update this program.
	/// </summary>
	static void update(Flags flags);
	/// <summary>
	/// Gives back the version of the program and all submodules.
	/// </summary>
	static void version(Flags flags);
	/// <summary>
	/// Prints a help message.
	/// </summary>
	static void help(std::string commandString);
};
