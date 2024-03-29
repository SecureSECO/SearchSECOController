/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "commands.h"


class CommandFactory
{
public:
	CommandFactory();
	
	/// <summary>
	/// Returns the command specified by the given string. Will always fail if init() is not called beforehand.
	/// </summary>
	Command* getCommand(std::string commandString);
	
	/// <summary>
	/// Prints general help message or a command specific help message.
	/// </summary>
	void printHelpMessage(std::string commandString);
private:
	std::map<std::string, Command*> comMap;
	std::string helpMessageCommonFlags;
};
