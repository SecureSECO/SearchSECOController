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

struct HashData;

class Commands
{
public:
	/// <summary>
	/// Will execute the given command with the flags you give it.
	/// </summary>
	/// <param name="command">The command you want to execute</param>
	/// <param name="flags">The flags with which you want to execute the command</param>
	static void execute(std::string command, Flags flags);

	/// <summary>
	/// Checks if there is a command with the given name.
	/// </summary>
	/// <param name="command">The command you want to check if it exists</param>
	static bool isCommand(std::string command);
private:
	static void downloadRepository(std::string repository, Flags flags, std::string downloadPath);
	static std::vector<HashData> parseRepository(std::string repository, Flags flags);
	static std::map<std::string, std::function<void(Flags)>> perform;

	static void start(Flags flags);
	static void check(Flags flags);
	static void upload(Flags flags);
	static void checkupload(Flags flags);
	static void update(Flags flags);
	static void version(Flags flags);
	static void help(Flags flags);
};
