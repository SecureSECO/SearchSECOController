/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Print.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <functional>

#pragma region Print

bool VerbosityAtLeast(utils::VerbosityLevel atLeast) 
{
	return (int)atLeast <= (int)print::verbosity;
}

void print::printline(std::string str)
{
	std::cout << str << '\n';
}

std::string print::tab(int n)
{
	std::string result = "";
	for (int i = 0; i < n; ++i)
	{
		result += "\t";
	}
	return result;
}

void print::help()
{
	print::printline("help section is yet to be implemented");
}

std::string print::encapsulate(std::string str, char c)
{
	return c + str + c;
}

std::string print::quote(std::string str)
{
	return encapsulate(str, '\"');
}

std::string print::plural(std::string singular, int n)
{
	if (n == 1) return singular;
	return singular + 's';
}

// Versioning
void print::version_full()
{
	std::string main_name = "searchseco";

	// Get subsystem versions
	int systemc = 2;
	std::string* subsystems = new std::string[systemc]
	{
		"parser",
		"spider",
		//"database_api"
	};

	std::ifstream version_file;
	std::string version;

	// print version of the main program
	version_file.open("VERSION");
	std::getline(version_file, version);

	print::printline(main_name + " version " + version);
	
	version_file.close();

	// Loop over the subsystems
	for (int i = 0; i < systemc; ++i)
	{
		std::string system = subsystems[i];
		version_file.open(system + "/VERSION");

		std::getline(version_file, version);

		print::printline(print::tab() + system + " version " + version);
		
		version_file.close();
	}
}

#pragma endregion Print

#pragma region Logging_Warning

// Logging and warning

void error::log(std::string str)
{
	if (VerbosityAtLeast(utils::VerbosityLevel::All)) {
		print::printline("L - " + str);
	}
}

void error::warn(int code)
{
	if (code <= 0) throw std::out_of_range("Argument \"code\" out of range in warn function.");

	if (VerbosityAtLeast(utils::VerbosityLevel::Warnings)) {
		std::string generic_warning_msg = "Generic warning message";
		print::printline("W" + std::to_string(code) + " - " + generic_warning_msg);
	}
}

#pragma endregion Logging_Warning

#pragma region Error_handling

int err_code_length = 3;

// Defines the (order of the) error codes for the various errors.
enum err_code
{
	flag_not_exist = 1,
	flag_not_exist_cfg,
	flag_invalid_arg,
	flag_invalid_arg_cfg,
	cmd_insufficient_args,
	cmd_not_found,
	cmd_not_exist,
	not_implemented,
};

// Descriptions of the error messages
#pragma region Descriptions

// strs: [commandname, expected_num_args, received_num_args]
std::string desc_cmd_insufficient_arguments(std::string* strs)
{
	return strs[1] + print::plural(" argument", std::stoi(strs[1])) + " expected for command " + print::quote(strs[0]) + ", received " + strs[2] + '.';
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
	return "Argument " + print::quote(strs[1]) + " is invalid for the flag " + print::quote("--" + strs[0]) + ".\n" + print::tab() + "See --help for valid value ranges.";
}

// strs: [flagname, argname]
std::string desc_err_flag_invalid_arg_cfg(std::string* strs)
{
	return "Argument " + print::quote(strs[1]) + " is invalid for the flag " + print::quote("--" + strs[0]) + " (configuration file).\n" + print::tab() + "See --help for valid value ranges.";
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

// Maps an error code to a description.
std::map <int, std::function<std::string(std::string*)>> err_desc =
{
	{flag_not_exist, desc_err_flag_not_exist},
	{flag_not_exist_cfg, desc_err_flag_not_exist_cfg},
	{flag_invalid_arg, desc_err_flag_invalid_arg},
	{flag_invalid_arg_cfg, desc_err_flag_invalid_arg_cfg},
	{cmd_insufficient_args, desc_cmd_insufficient_arguments},
	{cmd_not_found, desc_err_cmd_not_found},
	{cmd_not_exist, desc_err_cmd_not_exist},
	{not_implemented, desc_err_not_implemented},
};

#pragma endregion Descriptions

// MAIN ERROR FUNCTION
// Displays the actual error message, defined by its code, and then exits the program.
void err(err_code code, std::string* strs, std::string extra_msg = "")
{
	if (VerbosityAtLeast(utils::VerbosityLevel::Errors)) {
		print::printline("E" + utils::padLeft(std::to_string(code), '0', err_code_length) + " - " + err_desc[code](strs));
		if (extra_msg != "") print::printline(extra_msg);
	}
	delete[] strs;
	exit(EXIT_FAILURE);
}

#pragma region Specific_error_handlers

void error::err_insufficient_arguments(std::string command, int expected, int received)
{
	err(cmd_insufficient_args, new std::string[3] {command, std::to_string(expected), std::to_string(received)});
}

void error::err_flag_not_exist(std::string flag, bool from_config)
{
	std::string* flagname = new std::string[1] {flag};
	err_code code = from_config ? flag_not_exist_cfg : flag_not_exist;

	err(code, flagname);
}

void error::err_flag_invalid_arg(std::string flag, std::string arg, bool from_config)
{
	std::string* strs = new std::string[2]{ flag, arg };
	err_code code = from_config ? flag_invalid_arg_cfg : flag_invalid_arg;

	err(code, strs);
}

void error::err_cmd_not_found()
{
	err(cmd_not_found, {});
}

void error::err_cmd_not_exist(std::string command)
{
	err(cmd_not_exist, new std::string[1] {command});
}

void error::err_not_implemented(std::string funcname)
{
	err(not_implemented, new std::string[1] {funcname});
}

#pragma endregion Specific_error_handlers

#pragma endregion Error_handling
