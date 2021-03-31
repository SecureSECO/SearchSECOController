/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Input.h"

#include <iostream>
#include <vector>
#include <regex>

#include "Print.h"
#include "Parser.h"

Input::Input(int argc, char* argv[]) 
	: flags()
{
	Input::parseCliInput(argc, argv);

	Input::applyDefaults();

	Input::sanitizeArguments();
}

void Input::parseCliInput(int argc, char* argv[])
{
	std::string* args = new std::string[argc]();

	// For debugging in the IDE. Read another line if only 'searchseco' was entered.
	if (argc == 1)
	{
		std::string s;
		std::getline(std::cin, s);
		std::vector<std::string> temp = utils::split(s, ' ');
		delete[] args;
		args = new std::string[1 + temp.size()]();
		args[0] = argv[0];
		argc = 1 + temp.size();
		for (int i = 1; i < argc; i++)
		{
			args[i] = temp[i - 1];
		}
	}
	else
	{
		// Convert the arguments from char* to string
		for (int i = 0; i < argc; i++)
			args[i] = argv[i];
	}

	// Concatenate the flag/argument pairs, intercalated with spaces
	std::string flargs = "";
	for (int i = 1; i < argc; ++i)
		flargs += args[i] + ' ';

	Input::parseExecutablePath(args[0]);

	Input::parseOptionals(flargs);

	Flags::mapShortFlagToLong(this->optionalArguments);
}

void Input::parseExecutablePath(std::string fullPath)
{
	std::smatch match;
	std::regex pathRegex("(.+)searchseco.exe");

	std::regex_match(fullPath, match, pathRegex);

	this->executablePath = match[1];
}

void Input::parseOptionals(std::string flargs)
{
	// TODO throw error when the call is malformed (regex fails)
	std::smatch 
		syntaxMatch,
		flargMatch;

	std::regex	
		syntaxRegex("(?:([^-\\s]*)\\s)?(?:([^-\\s]*)\\s)?(.*)"),
		flargRegex("(?:(?:-([^-\\s]+)))\\s?([^-\\s]+)?");

	// Validate the syntax of the call
	std::regex_match(flargs, syntaxMatch, syntaxRegex);

	this->command = syntaxMatch[1];
	this->flags.mandatoryArgument = syntaxMatch[2];
	std::string rest = syntaxMatch[3];

	// Parse optional flags
	this->optionalArguments = {};

	std::string::const_iterator searchStart(rest.cbegin());
	while (regex_search(searchStart, rest.cend(), flargMatch, flargRegex))
	{
		std::string 
			flag = flargMatch[1],
			arg = flargMatch[2];

		if (!Flags::isFlag(flag)) error::err_flag_not_exist(flag, false);


		this->optionalArguments[flag] = arg;

		searchStart = flargMatch.suffix().first;
	}
}

void Input::applyDefaults()
{
	std::map<std::string, std::string> fullArgs = {};
	this->flagSource = {};

	std::map<std::string, std::string> configDefaults = FlagParser::parseConfig(this->executablePath + "/cfg/config.txt");

	std::map<std::string, std::string>::iterator it;
	for (it = configDefaults.begin(); it != configDefaults.end(); ++it)
	{
		std::string flag = it->first;
		fullArgs[flag] = configDefaults[flag];
		this->flagSource[flag] = "config";
	}

	for (it = this->optionalArguments.begin(); it != this->optionalArguments.end(); ++it)
	{
		std::string flag = it->first;
		fullArgs[flag] = this->optionalArguments[flag];
		this->flagSource[flag] = "cli_input";
	}

	this->optionalArguments = fullArgs;
}

void Input::sanitizeArguments()
{
	std::map<std::string, std::string>::iterator it;
	for (it = this->optionalArguments.begin(); it != this->optionalArguments.end(); ++it)
	{
		std::string flag = it->first;
		std::string argument = it->second;
		bool fromConfig = this->flagSource[flag] == "config";

		if (flag == "cpu")
			Input::validateInteger(
				argument, 
				[this](int x) { this->flags.flag_cpu = x; },
				[flag, argument, fromConfig]() { error::err_flag_invalid_arg(flag, argument, fromConfig); },
				2);
		else if (flag == "ram")
			Input::validateInteger(
				argument, 
				[this](int x) { this->flags.flag_ram = x; }, 
				[flag, argument, fromConfig]() { error::err_flag_invalid_arg(flag, argument, fromConfig); },
				4, 
				64);
		else if (flag == "output")
		{
			// TODO implement flag validation
		}
		else if (flag == "save")
			Input::validateBoolean(
				argument,
				[this]() { this->flags.flag_save = true; },
				[flag]() { error::err_flag_incorrect_arguments(flag, 0, 1); }
			);
		else if (flag == "verbose")
		{
			// TODO implement flag validation
		}
		else if (flag == "help") 
			Input::validateBoolean(
				argument,
				[this]() { this->flags.flag_help = true; },
				[flag]() { error::err_flag_incorrect_arguments(flag, 0, 1); }
			);
		else if (flag == "version") 
			Input::validateBoolean(
				argument,
				[this]() { this->flags.flag_version = true; },
				[flag]() { error::err_flag_incorrect_arguments(flag, 0, 1); }
		);
	}
}

template <typename Callback, typename Error>
void Input::validateInteger(std::string argument, Callback callback, Error error, int min, int max)
{
	if (utils::isNumber(argument))
	{
		int flag_int = std::stoi(argument);
		if (flag_int < 2) error();
		callback(flag_int);
	}
	else
		error();
}

template <typename Callback, typename Error>
void Input::validateBoolean(std::string argument, Callback callback, Error error)
{
	if (argument != "") error();

	callback();
}