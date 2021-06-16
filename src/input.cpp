/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "input.h"
#include "print.h"
#include "regexWrapper.h"

// External includes
#include <filesystem>
#include <iostream>
#include <regex>
#include <vector>


#define CONFIGURATION_FILE "cfg/config.txt"

Input::Input(int argc, char* argv[]) 
	: flags()
{
    print::debug("Parsing CLI input", __FILE__, __LINE__);
	Input::parseCliInput(argc, argv);

	print::debug("Applying default arguments to flags", __FILE__, __LINE__);
	Input::applyDefaults();

	print::debug("Sanitizing provided arguments", __FILE__, __LINE__);
	Input::sanitizeArguments();
}

void Input::parseCliInput(int argc, char* argv[])
{
	std::string* args = new std::string[argc]();

	// For debugging in the IDE. Read another line if only 'searchseco' was entered.
	if (argc == 1)
	{
        print::warn("Local debugging detected", __FILE__, __LINE__);
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
		{
			args[i] = argv[i];
		}
	}

	// Concatenate the flag/argument pairs, intercalated with spaces.
	std::string flargs = "";
	for (int i = 1; i < argc; ++i)
	{
		flargs += args[i] + ' ';
	}

	print::debug("Finding executable path", __FILE__, __LINE__);
	Input::getExecutablePath();

	print::debug("Parsing optionals", __FILE__, __LINE__);
	Input::parseOptionals(flargs);

	print::debug("Mapping shorthand flags to longer versions", __FILE__, __LINE__);
	Flags::mapShortFlagToLong(this->optionalArguments);
}

void Input::getExecutablePath()
{
	this->executablePath = std::filesystem::current_path().string();

	print::debug("Found executable path as " + print::quote(this->executablePath), __FILE__, __LINE__);
}

void Input::parseOptionals(std::string call)
{
	// Validate the syntax of the call.
	std::tuple<std::string, std::string, std::string> result;
	bool valid = regex::validateSyntax(call, result);

	if (!valid)
	{
		error::errParseCallSyntaxError(call, __FILE__, __LINE__);
	}

	this->command = std::get<0>(result);
	print::debug("Parsed command as " + print::quote(this->command), __FILE__, __LINE__);
	this->flags.mandatoryArgument = std::get<1>(result);
	if (flags.mandatoryArgument == "")
	{
		print::debug("No mandatory argument was entered", __FILE__, __LINE__);
	}
	else 
	{
		print::debug("Parsed mandatory argument as " + print::quote(this->flags.mandatoryArgument), __FILE__, __LINE__);
	}
	
	std::string flargs = std::get<2>(result);

	if (flags.mandatoryArgument != "" && !regex::validateURL(flags.mandatoryArgument))
	{
		error::errInvalidUrl(flags.mandatoryArgument, __FILE__, __LINE__);
	}

	// Parse optional flags.
	this->optionalArguments = {};

	std::map<std::string, std::string> flagMap;
	regex::parseFlargPairs(flargs, flagMap);

	for (auto const& pair : flagMap)
	{
		std::string
			flag     = pair.first,
			argument = pair.second;

		if (!Flags::isFlag(flag))
		{
			error::errFlagNotExist(flag, false, __FILE__, __LINE__);
		}

		print::debug("Detected flag " + print::quote(flag) + " with argument " + print::quote(argument), __FILE__, __LINE__);

		this->optionalArguments[flag] = argument;
	}
}

void Input::applyDefaults()
{
	std::map<std::string, std::string> fullArgs = {};
	this->flagSource = {};

	auto configpath = (std::filesystem::path(this->executablePath) / std::string(CONFIGURATION_FILE))
		.make_preferred()
		.string();

	print::debug("Reading config file at " + configpath, __FILE__, __LINE__);
	std::map<std::string, std::string> configDefaults = Flags::parseConfig(configpath);

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
	print::debug("Default argument values applied successfully", __FILE__, __LINE__);
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
		else if (flag == "branch")
		{
			Input::sanitizeBranchFlag(argument, fromConfig);
		}
		else if (flag == "github_username")
		{
			Input::sanitizeGithubUsername(argument, fromConfig);
		}
		else if (flag == "github_token")
		{
			Input::sanitizeGithubToken(argument, fromConfig);
		}
	}
}

#pragma region Individual flag sanitation
void Input::sanitizeCpuFlag(std::string arg, bool fromConfig)
{
	auto msg = "Sanitizing --cpu flag with argument " + arg 
		+ (fromConfig ? " from the config file" : "");

	print::debug(msg, __FILE__, __LINE__);
	Input::requireNArguments(1, "cpu", arg);
	Input::validateInteger(
		arg,
		[this](int x) { this->flags.flag_cpu = x; },
		[arg, fromConfig]() { error::errFlagInvalidArg("cpu", arg, fromConfig, __FILE__, __LINE__); },
		2);
}

void Input::sanitizeVerboseFlag(std::string arg, bool fromConfig)
{
	auto msg = "Sanitizing --verbose flag with argument " + arg
		+ (fromConfig ? " from the config file" : "");

	print::debug(msg, __FILE__, __LINE__);
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
		[arg, fromConfig]() { error::errFlagInvalidArg("verbose", arg, fromConfig, __FILE__, __LINE__); },
		1,
		5
	);
}

void Input::sanitizeHelpFlag(std::string arg, bool fromConfig)
{
	auto msg = "Sanitizing --help flag";

	print::debug(msg, __FILE__, __LINE__);
	Input::requireNArguments(0, "help", arg);
	this->flags.flag_help = true;
}

void Input::sanitizeVersionFlag(std::string arg, bool fromConfig)
{
	auto msg = "Sanitizing --version flag";

	print::debug(msg, __FILE__, __LINE__);
	Input::requireNArguments(0, "version", arg);
	this->flags.flag_version = true;
}

void Input::sanitizeBranchFlag(std::string arg, bool fromConfig)
{
	auto msg = "Sanitizing --branch flag";

	print::debug(msg, __FILE__, __LINE__);
	Input::requireNArguments(1, "branch", arg);

	// TODO: Maybe check if this is something which could be a legal branch?
	this->flags.flag_branch = arg;
}

void Input::sanitizeGithubUsername(std::string arg, bool fromConfig)
{
	auto msg = "Sanitizing --github_username flag";

	print::debug(msg, __FILE__, __LINE__);
	if (arg == "")
	{
		print::warn("Missing github Username, functionality will be limited", __FILE__, __LINE__);
	}

	this->flags.flag_github_user = arg;
}

void Input::sanitizeGithubToken(std::string arg, bool fromConfig)
{
	auto msg = "Sanitizing --github_token flag";

	print::debug(msg, __FILE__, __LINE__);
	if (arg == "")
	{
		print::warn("Missing github token, functionality will be limited", __FILE__, __LINE__);
	}

	this->flags.flag_github_token = arg;
}
#pragma endregion

void Input::requireNArguments(int n, std::string flag, std::string argument)
{
	if (n == 0 && argument != "")
	{
		error::errFlagIncorrectArguments(flag, n, 1, __FILE__, __LINE__);
	}

	if (n == 1 && argument == "")
	{
		error::errFlagIncorrectArguments(flag, n, 0, __FILE__, __LINE__);
	}
}

template <typename callbackFunction, typename errorFunction>
void Input::validateInteger(std::string argument, callbackFunction callback, errorFunction error, int min, int max)
{
	if (utils::isNumber(argument))
	{
		int flagInt = std::stoi(argument);
		if (flagInt < min || flagInt > max) 
		{ 
			error(); 
		}

		callback(flagInt);
	}
	else
	{
		error();
	}
}
