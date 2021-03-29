/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <regex>
#include "Parser.h"
#include "Commands.h"
#include "Print.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
	std::string* args = new std::string[argc]();

	// check if arguments are provided. If not, read them from the console
	if (argc == 1) //check pathOrSomething  -s t
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

	// make string of all arguments (except for the path)
	std::string flargs = "";
	for (int i = 1; i < argc; ++i)
	{
		flargs += args[i] + ' ';
	}


	// regex constants
	std::smatch syntaxMatch,
				flargMatch;

	std::regex	syntaxRegex("(\\S*)\\s(?:([^-\\s]*)\\s)?(.*)");

	// match base
	std::regex_match(flargs, syntaxMatch, syntaxRegex);

	std::string command = syntaxMatch[1],
				mandatory_arguments = syntaxMatch[2],
				rest = syntaxMatch[3];

	// parse optional flags
	std::regex flargRegex("(?:(?:-([^-\\s]+)))\\s?([^-\\s]+)?");

	std::map<std::string, std::string> optional_arguments = {};

	std::string::const_iterator searchStart( rest.cbegin() );
    while ( regex_search( searchStart, rest.cend(), flargMatch, flargRegex ) )
    {
		std::string flag = flargMatch[1],
					arg = flargMatch[2];

		optional_arguments[flag] = arg;

		// TODO: error, warning or something upon non matching string

        searchStart = flargMatch.suffix().first;
    }


	std::string location = args[0];
	// Getting all the flags and arguments out of the config file and command line arguments
	// TODO make config flexible?
	std::map<std::string, std::string> flagArgs = FlagParser::parse("config.txt", location, command, mandatory_arguments, optional_arguments);

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
