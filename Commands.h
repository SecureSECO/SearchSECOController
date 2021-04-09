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

struct HashData;

class Commands
{
public:
	/// <summary>
	/// Will execute the given command with the flags you give it.
	/// </summary>
	/// <param name="command">The command you want to execute</param>
	/// <param name="flags">The flags with which you want to execute the command</param>
	static void execute(std::string command, std::map<std::string, std::string> flags);

	/// <summary>
	/// Checks if there is a command with the given name.
	/// </summary>
	/// <param name="command">The command you want to check if it exists</param>
	static bool isCommand(std::string command);
private:
	/// <summary>
	/// Will call the spider to download a given repository.
	/// </summary>
	static std::string downloadRepository(std::string repository, 
		std::map<std::string, std::string> flags, std::string downloadPath);
	/// <summary>
	/// Will call the parser to parse the given repository.
	/// </summary>
	static std::vector<HashData> parseRepository(std::string repository, std::map<std::string, std::string> flags);
	
	static std::map<std::string, std::function<void(std::map<std::string, std::string>)>> perform;
	static std::map<std::string, std::string> helpMessagesCommands;
	static std::vector<std::string> commandNames;
	static std::map<std::string, std::string> helpMessagesCommonFlags;
	static std::vector<std::string> commonFlagNames;

	static void start(std::map<std::string, std::string> flags);
	static void check(std::map<std::string, std::string> flags);
	static void upload(std::map<std::string, std::string> flags);
	static void checkupload(std::map<std::string, std::string> flags);
	static void update(std::map<std::string, std::string> flags);
	static void version(std::map<std::string, std::string> flags);
	static void help(std::map<std::string, std::string> flags);
};
