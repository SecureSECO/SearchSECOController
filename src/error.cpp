/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "print.h"
#include "utils.h"

// External includes
#include "loguru/loguru.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <stdlib.h>


#pragma region Error_messages

int errCodeLength = 3;

// Defines the (order of the) error codes for the various errors.
enum errCode
{
	flagNotExist = 1,
	flagNotExistCfg,
	flagInvalidArg,
	flagInvalidArgCfg,
	flagIncorrectArgs,
	cmdIncorrectArgs,
	cmdNotFound,
	cmdNotExist,
	parseCallSyntaxError,
	parseIncorrectShorthandFlag,
	parseIncorrectLonghandFlag,
	parseCouldNotParseFlag,
	invalidUrl,
	notImplemented,
};

// Descriptions of the error messages.
#pragma region Descriptions

// strs: [commandname, expected_num_args, received_num_args]
std::string descCmdIncorrectArguments(std::string* strs)
{
	return strs[1] + print::plural(" argument", std::stoi(strs[1])) + " expected for command " + print::quote(strs[0]) + ", received " + strs[2] + '.';
}

// strs: [commandname, expected_num_args, received_num_args]
std::string descFlagIncorrectArguments(std::string* strs)
{
	return strs[1] + print::plural(" argument", std::stoi(strs[1])) + " expected for flag " + print::quote("--" + strs[0]) + ", received " + strs[2] + '.';
}

// strs: [flagname]
std::string descErrFlagNotExist(std::string* strs)
{
	return "The flag " + print::quote(strs[0]) + " does not exist.";
}

// strs: [flagname]
std::string descErrFlagNotExistCfg(std::string* strs)
{
	return "The flag " + print::quote(strs[0]) + " does not exist (configuration file).";
}

// strs: [flagname, argname]
std::string descErrFlagInvalidArg(std::string* strs)
{
	return "Argument " + print::quote(strs[1]) + " is invalid for the flag " + print::quote("--" + strs[0]) + ". See --help (-h) for valid value ranges.";
}

// strs: [flagname, argname]
std::string descErrFlagInvalidArgCfg(std::string* strs)
{
	return "Argument " + print::quote(strs[1]) + " is invalid for the flag " + print::quote("--" + strs[0]) + " (configuration file).\n" + ". See --help (-h) for valid value ranges.";
}

// no strs
std::string descErrCmdNotFound(std::string* strs)
{
	return "No command was entered.";
}

// strs: [commandname]
std::string descErrCmdNotExist(std::string* strs)
{
	return "Command " + print::quote(strs[0]) + " does not exist.";
}

// strs: [funcname]
std::string descErrNotImplemented(std::string* strs)
{
	return "The function " + print::quote(strs[0]) + " is not yet implemented.";
}

// strs: [callstring]
std::string descParseCallSyntaxError(std::string* strs)
{
	return "Error while parsing call string " + print::quote(strs[0]);
}

// strs: [flagname]
std::string descParseIncorrectShorthandFlag(std::string* strs)
{
	return "Flag " + print::quote("--" + strs[0]) + " was incorrectly entered as if it were a full-length flag (" 
		"suggestion: " + print::quote("-" + strs[0]) + ")";
}

// strs: [flagname]
std::string descParseIncorrectLonghandFlag(std::string* strs)
{
	return "Flag " + print::quote("-" + strs[0]) + " was incorrectly entered as if it were a shorthand flag ("
		"suggestion: " + print::quote("--" + strs[0]) + ")";
}

// strs: [url]
std::string descInvalidUrl(std::string* strs)
{
	return print::quote(strs[0]) + " is not a valid URL";
}

// strs: [flagname]
std::string descParseCouldNotParseFlag(std::string* strs)
{
	return strs[0] + " could not be parsed";
}

// Maps an error code to a description.
std::map <int, std::function<std::string(std::string*)>> errDesc =
{
	{flagNotExist, descErrFlagNotExist},
	{flagNotExistCfg, descErrFlagNotExistCfg},
	{flagInvalidArg, descErrFlagInvalidArg},
	{flagInvalidArgCfg, descErrFlagInvalidArgCfg},
	{flagIncorrectArgs, descFlagIncorrectArguments},
	{cmdIncorrectArgs, descCmdIncorrectArguments},
	{cmdNotFound, descErrCmdNotFound},
	{cmdNotExist, descErrCmdNotExist},
	{parseCallSyntaxError, descParseCallSyntaxError},
	{parseIncorrectShorthandFlag, descParseIncorrectShorthandFlag},
	{parseIncorrectLonghandFlag, descParseIncorrectLonghandFlag},
	{parseCouldNotParseFlag, descParseCouldNotParseFlag},
	{invalidUrl, descInvalidUrl},
	{notImplemented, descErrNotImplemented},
};

#pragma endregion Descriptions

// MAIN ERROR FUNCTION
// Displays the actual error message, defined by its code, and then exits the program.
void err(errCode code, std::string* strs, const char* file, int line, std::string extraMsg = "")
{
	std::string msg = "E" + utils::padLeft(std::to_string(code), '0', errCodeLength) + " - " + errDesc[code](strs);

	loguru::log(loguru::Verbosity_ERROR, file, line, "%s", msg.c_str());

	if (extraMsg != "") 
	{ 
		print::log(extraMsg, file, line); 
	}

	delete[] strs;

	loguru::g_stderr_verbosity = loguru::Verbosity_OFF;

	exit(EXIT_FAILURE);
}

#pragma region Specific_error_handlers

void error::errCmdIncorrectArguments(std::string command, int expected, int received, const char* file, int line)
{
	err(cmdIncorrectArgs,
		new std::string[3]{
			command,
			std::to_string(expected),
			std::to_string(received) },
			file, line
			);
}

void error::errFlagIncorrectArguments(std::string flag, int expected, int received, const char* file, int line)
{
	err(flagIncorrectArgs,
		new std::string[3]{
			flag,
			std::to_string(expected),
			std::to_string(received) },
			file, line
			);
}

void error::errFlagNotExist(std::string flag, bool fromConfig, const char* file, int line)
{
	std::string* flagname = new std::string[1]{ flag };
	errCode code = fromConfig ? flagNotExistCfg : flagNotExist;

	err(code, flagname, file, line);
}

void error::errFlagInvalidArg(std::string flag, std::string arg, bool fromConfig, const char* file, int line)
{
	std::string* strs = new std::string[2]{ flag, arg };
	errCode code = fromConfig ? flagInvalidArgCfg : flagInvalidArg;

	err(code, strs, file, line);
}

void error::errCmdNotFound(const char* file, int line)
{
	err(cmdNotFound, {}, file, line);
}

void error::errCmdNotExist(std::string command, const char* file, int line)
{
	err(cmdNotExist,
		new std::string[1]{ command },
		file, line
	);
}

void error::errParseCallSyntaxError(std::string callstring, const char* file, int line)
{
	err(parseCallSyntaxError,
		new std::string[1]{ callstring },
		file, line
	);
}

void error::errParseIncorrectShorthandFlag(std::string flag, const char* file, int line)
{
	err(parseIncorrectShorthandFlag,
		new std::string[1] { flag },
		file, line
	);
}

void error::errParseIncorrectLonghandFlag(std::string flag, const char* file, int line)
{
	err(parseIncorrectLonghandFlag,
		new std::string[1] { flag },
		file, line
	);
}

void error::errParseCouldNotParseFlag(std::string flag, const char* file, int line)
{
	err(parseCouldNotParseFlag,
		new std::string[1] { flag },
		file, line
	);
}

void error::errInvalidUrl(std::string url, const char* file, int line)
{
	err(invalidUrl,
		new std::string[1] { url },
		file, line
	);
}

void error::errNotImplemented(std::string funcname, const char* file, int line)
{
	err(notImplemented,
		new std::string[1]{ funcname },
		file, line
	);
}

#pragma endregion Specific_error_handlers

#pragma endregion Error_messages