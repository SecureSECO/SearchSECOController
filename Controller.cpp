/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "FlagParser.h"
#include "Commands.h"
#include "Print.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
	std::string* args = new std::string[argc]();

	if (argc == 1) //check pathOrSomething -s t
	{
		std::string s;
		std::getline(std::cin, s);
		std::vector<std::string> temp = utils::split(s, ' ');
		delete[] args;
		args = new std::string[1+temp.size()]();
		args[0] = argv[0];
		argc = 1 + temp.size();
		for (int i = 1; i < argc; i++)
		{
			args[i] = temp[i-1];
		}
	}
	else
	{
		// Converting the command line arguments from char* to string
		for (int i = 0; i < argc; i++)
		{
			args[i] = argv[i];
		}
	}

	// Getting all the flags and arguments out of the config file and command line arguments
	std::map<std::string, std::string> flagArgs = FlagParser::parse("config.txt", args, argc);

	// Checking if a command was given
	if (flagArgs.count("command") == 0)
	{
		error::err_cmd_not_found();
	}
	// Checking if the command is one we actually know, and then execution it
	if (Commands::isCommand(flagArgs["command"]))
	{
		Commands::execute(flagArgs["command"], flagArgs);
	}
	else
	{
		error::err_cmd_not_exist(flagArgs["command"]);
	}
}
