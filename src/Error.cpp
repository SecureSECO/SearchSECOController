/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "loguru/loguru.hpp"

#include "Print.h"
#include "Utils.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <stdlib.h>

#pragma region Error_messages

int err_code_length = 3;

// Defines the (order of the) error codes for the various errors.
enum err_code
{
	flag_not_exist = 1,
	flag_not_exist_cfg,
	flag_invalid_arg,
	flag_invalid_arg_cfg,
	flag_incorrect_args,
	cmd_incorrect_args,
	cmd_not_found,
	cmd_not_exist,
	parse_call_syntax_error,
	parse_incorrect_shorthand_flag,
	parse_incorrect_longhand_flag,
	parse_could_not_parse_flag,
	not_implemented,
};

// Descriptions of the error messages.
#pragma region Descriptions

// strs: [commandname, expected_num_args, received_num_args]
std::string desc_cmd_incorrect_arguments(std::string* strs)
{
	return strs[1] + print::plural(" argument", std::stoi(strs[1])) + " expected for command " + print::quote(strs[0]) + ", received " + strs[2] + '.';
}

// strs: [commandname, expected_num_args, received_num_args]
std::string desc_flag_incorrect_arguments(std::string* strs)
{
	return strs[1] + print::plural(" argument", std::stoi(strs[1])) + " expected for flag " + print::quote("--" + strs[0]) + ", received " + strs[2] + '.';
}

// strs: [flagname]
std::string desc_err_flag_not_exist(std::string* strs)
{
	return "The flag " + print::quote(strs[0]) + " does not exist.";
}

// strs: [flagname]
std::string desc_err_flag_not_exist_cfg(std::string* strs)
{
	return "The flag " + print::quote(strs[0]) + " does not exist (configuration file).";
}

// strs: [flagname, argname]
std::string desc_err_flag_invalid_arg(std::string* strs)
{
	return "Argument " + print::quote(strs[1]) + " is invalid for the flag " + print::quote("--" + strs[0]) + ". See --help (-h) for valid value ranges.";
}

// strs: [flagname, argname]
std::string desc_err_flag_invalid_arg_cfg(std::string* strs)
{
	return "Argument " + print::quote(strs[1]) + " is invalid for the flag " + print::quote("--" + strs[0]) + " (configuration file).\n" + ". See --help (-h) for valid value ranges.";
}

// no strs
std::string desc_err_cmd_not_found(std::string* strs)
{
	return "No command was entered.";
}

// strs: [commandname]
std::string desc_err_cmd_not_exist(std::string* strs)
{
	return "Command " + print::quote(strs[0]) + " does not exist.";
}

// strs: [funcname]
std::string desc_err_not_implemented(std::string* strs)
{
	return "The function " + print::quote(strs[0]) + " is not yet implemented.";
}

// strs: [callstring]
std::string desc_parse_call_syntax_error(std::string* strs)
{
	return "Error while parsing call string " + print::quote(strs[0]);
}

// strs: [flagname]
std::string desc_parse_incorrect_shorthand_flag(std::string* strs)
{
	return "Flag " + print::quote("--" + strs[0]) + " was incorrectly entered as if it were a shorthand flag (" 
		+ print::quote("-" + strs[0]) + ")";
}

// strs: [flagname]
std::string desc_parse_incorrect_longhand_flag(std::string* strs)
{
	return "Flag " + print::quote("-" + strs[0]) + " was incorrectly entered as if it were a full-length flag ("
		+ print::quote("--" + strs[0]) + ")";
}

// strs: [flagname]
std::string desc_parse_could_not_parse_flag(std::string* strs)
{
	return strs[0] + " could not be parsed";
}

// Maps an error code to a description.
std::map <int, std::function<std::string(std::string*)>> err_desc =
{
	{flag_not_exist, desc_err_flag_not_exist},
	{flag_not_exist_cfg, desc_err_flag_not_exist_cfg},
	{flag_invalid_arg, desc_err_flag_invalid_arg},
	{flag_invalid_arg_cfg, desc_err_flag_invalid_arg_cfg},
	{flag_incorrect_args, desc_flag_incorrect_arguments},
	{cmd_incorrect_args, desc_cmd_incorrect_arguments},
	{cmd_not_found, desc_err_cmd_not_found},
	{cmd_not_exist, desc_err_cmd_not_exist},
	{not_implemented, desc_err_not_implemented},
};

#pragma endregion Descriptions

// MAIN ERROR FUNCTION
// Displays the actual error message, defined by its code, and then exits the program.
void err(err_code code, std::string* strs, const char* file, int line, std::string extra_msg = "")
{
	std::string msg = "E" + utils::padLeft(std::to_string(code), '0', err_code_length) + " - " + err_desc[code](strs);

	loguru::log(loguru::Verbosity_ERROR, file, line, msg.c_str());

	if (extra_msg != "") print::log(extra_msg, file, line);
	delete[] strs;
	exit(EXIT_FAILURE);
}

#pragma region Specific_error_handlers

void error::err_cmd_incorrect_arguments(std::string command, int expected, int received, const char* file, int line)
{
	err(cmd_incorrect_args,
		new std::string[3]{
			command,
			std::to_string(expected),
			std::to_string(received) },
			file, line
			);
}

void error::err_flag_incorrect_arguments(std::string flag, int expected, int received, const char* file, int line)
{
	err(flag_incorrect_args,
		new std::string[3]{
			flag,
			std::to_string(expected),
			std::to_string(received) },
			file, line
			);
}

void error::err_flag_not_exist(std::string flag, bool from_config, const char* file, int line)
{
	std::string* flagname = new std::string[1]{ flag };
	err_code code = from_config ? flag_not_exist_cfg : flag_not_exist;

	err(code, flagname, file, line);
}

void error::err_flag_invalid_arg(std::string flag, std::string arg, bool from_config, const char* file, int line)
{
	std::string* strs = new std::string[2]{ flag, arg };
	err_code code = from_config ? flag_invalid_arg_cfg : flag_invalid_arg;

	err(code, strs, file, line);
}

void error::err_cmd_not_found(const char* file, int line)
{
	err(cmd_not_found, {}, file, line);
}

void error::err_cmd_not_exist(std::string command, const char* file, int line)
{
	err(cmd_not_exist,
		new std::string[1]{ command },
		file, line
	);
}

void error::err_parse_call_syntax_error(std::string callstring, const char* file, int line)
{
	err(parse_call_syntax_error,
		new std::string[1]{ callstring },
		file, line
	);
}

void error::err_parse_incorrect_shorthand_flag(std::string flag, const char* file, int line)
{
	err(parse_incorrect_shorthand_flag,
		new std::string[1] { flag },
		file, line
	);
}

void error::err_parse_incorrect_longhand_flag(std::string flag, const char* file, int line)
{
	err(parse_incorrect_longhand_flag,
		new std::string[1] { flag },
		file, line
	);
}

void error::err_parse_could_not_parse_flag(std::string flag, const char* file, int line)
{
	err(parse_could_not_parse_flag,
		new std::string[1] { flag },
		file, line
	);
}

void error::err_not_implemented(std::string funcname, const char* file, int line)
{
	err(not_implemented,
		new std::string[1]{ funcname },
		file, line
	);
}

#pragma endregion Specific_error_handlers

#pragma endregion Error_messages
