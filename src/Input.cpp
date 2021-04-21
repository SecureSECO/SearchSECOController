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
#include "Parser2.h"
#include "loguru/loguru.hpp"

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
		// Convert the arguments from char* to string.
		for (int i = 0; i < argc; i++)
			args[i] = argv[i];
	}

	// Concatenate the flag/argument pairs, intercalated with spaces.
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
	// TODO throw error when the call is malformed (regex fails).
	std::smatch 
		syntaxMatch,
		flargMatch;

	std::regex	
		syntaxRegex("(?:([^-][^\\s]*)\\s)?(?:([^-][^\\s]*)\\s?)?(.*)"),
		flargRegex("(?:(?:-([^-\\s]+)))\\s?([^-\\s]+)?");

	// Validate the syntax of the call.
	std::regex_match(flargs, syntaxMatch, syntaxRegex);

	this->command = syntaxMatch[1];
	this->flags.mandatoryArgument = syntaxMatch[2];
	std::string rest = syntaxMatch[3];

	// Parse optional flags.
	this->optionalArguments = {};

	std::string::const_iterator searchStart(rest.cbegin());
	while (regex_search(searchStart, rest.cend(), flargMatch, flargRegex))
	{
		std::string 
			flag = flargMatch[1],
			arg = flargMatch[2];

		if (!Flags::isFlag(flag)) error::err_flag_not_exist(flag, false, __FILE__, __LINE__);


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
		{
			Input::sanitizeCpuFlag(argument, fromConfig);
		}
		else if (flag == "ram")
		{
			Input::sanitizeRamFlag(argument, fromConfig);
		}
		else if (flag == "output")
		{
			Input::sanitizeOutputFlag(argument, fromConfig);
		}
		else if (flag == "save")
		{
			Input::sanitizeSaveFlag(argument, fromConfig);
		}
		else if (flag == "verbose")
		{
			Input::sanitizeVerboseFlag(argument, fromConfig);
		}
		else if (flag == "help") 
		{
			Input::sanitizeHelpFlag(argument, fromConfig);
		}
		else if (flag == "version")
		{
			Input::sanitizeVersionFlag(argument, fromConfig);
		}
	}
}

#pragma region Individual flag sanitation
void Input::sanitizeCpuFlag(std::string arg, bool fromConfig)
{
	Input::requireNArguments(1, "cpu", arg);
	Input::validateInteger(
		arg,
		[this](int x) { this->flags.flag_cpu = x; },
		[arg, fromConfig]() { error::err_flag_invalid_arg("cpu", arg, fromConfig, __FILE__, __LINE__); },
		2);
}

void Input::sanitizeRamFlag(std::string arg, bool fromConfig)
{
	Input::requireNArguments(1, "ram", arg);
	Input::validateInteger(
		arg,
		[this](int x) { this->flags.flag_ram = x; },
		[arg, fromConfig]() { error::err_flag_invalid_arg("ram", arg, fromConfig, __FILE__, __LINE__); },
		4,
		64);
}

void Input::sanitizeOutputFlag(std::string arg, bool fromConfig)
{
	Input::requireNArguments(1, "output", arg);

	std::regex urlRegex("^(http:\\/\\/www\\.|https:\\/\\/www\\.|http:\\/\\/|https:\\/\\/)?[a-z0-9]+([\\-\\.]{1}[a-z0-9]+)*\\.[a-z]{2,5}(:[0-9]{1,5})?(\\/.*)?$");

	if (arg == "console" || std::regex_match(arg, urlRegex))
	{
		this->flags.flag_output = arg;
	}
	else error::err_flag_invalid_arg("output", arg, fromConfig, __FILE__, __LINE__);
}

void Input::sanitizeSaveFlag(std::string arg, bool fromConfig)
{
	Input::requireNArguments(0, "save", arg);
	this->flags.flag_save = true;
}

void Input::sanitizeVerboseFlag(std::string arg, bool fromConfig)
{
	Input::requireNArguments(1, "verbose", arg);

	std::map<int, loguru::Verbosity> verbosityMap =
	{
		{1, loguru::Verbosity_OFF},			// Quiet
		{2, loguru::Verbosity_ERROR},		// Only errors
		{3, loguru::Verbosity_WARNING},		// Errors & Warnings
		{4, loguru::Verbosity_INFO},		// All user-relevant logs
		{5, loguru::Verbosity_1},			// Everything, including debug messages
	};

	Input::validateInteger(
		arg,
		[this, verbosityMap](int x) { this->flags.flag_verbose = verbosityMap.at(x); },
		[arg, fromConfig]() { error::err_flag_invalid_arg("verbose", arg, fromConfig, __FILE__, __LINE__); },
		1,
		5
	);
}

void Input::sanitizeHelpFlag(std::string arg, bool fromConfig)
{
	Input::requireNArguments(0, "help", arg);
	this->flags.flag_help = true;
}

void Input::sanitizeVersionFlag(std::string arg, bool fromConfig)
{
	Input::requireNArguments(0, "version", arg);
	this->flags.flag_version = true;
}
#pragma endregion

void Input::requireNArguments(int n, std::string flag, std::string argument)
{
	if (n == 0 && argument != "") error::err_flag_incorrect_arguments(flag, n, 1, __FILE__, __LINE__);
	if (n == 1 && argument == "") error::err_flag_incorrect_arguments(flag, n, 0, __FILE__, __LINE__);

}

template <typename Callback, typename Error>
void Input::validateInteger(std::string argument, Callback callback, Error error, int min, int max)
{
	if (utils::isNumber(argument))
	{
		int flag_int = std::stoi(argument);
		if (flag_int < min || flag_int > max) error();
		callback(flag_int);
	}
	else
		error();
}
