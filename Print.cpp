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

void print::help(std::string message)
{
	print::printline(message);
	print::help();
}

std::string print::encapsulate(std::string str, char c)
{
	return c + str + c;
}

std::string print::quote(std::string str)
{
	return encapsulate(str, '\"');
}
std::string print::text_then_quote(std::string str, std::string q)
{
	return encapsulate(str, ' ') + quote(q);
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

// Logging and warning

void error::log(std::string str)
{
	print::printline("L - " + str);
}

void error::warn(int code)
{
	if (code <= 0) throw std::out_of_range("Argument \"code\" out of range in warn function.");

	std::string generic_warning_msg = "Generic warning message";
	print::printline("W" + std::to_string(code) + " - " + generic_warning_msg);
}

// Error handling

int err_code_length = 3;

// Defines the (order of the) error codes for the various errors.
enum err_code
{
	flag_not_exist = 1,
	flag_not_exist_cfg,
	flag_invalid_arg,
	flag_invalid_arg_cfg,
	cmd_invalid,
	cmd_insufficient_args,
	cmd_not_found,
	cmd_not_exist,
	not_implemented,
};

// Maps an error code to a description.
std::map <int, std::string> err_msg =
{
	{flag_not_exist, "The specified flag does not exist."},
	{flag_not_exist_cfg, "The specified flag does not exist in the config file."},
	{flag_invalid_arg, "The specified argument is invalid for its corresponding flag."},
	{flag_invalid_arg_cfg, "The specified argument is invalid for its corresponding flag in the config file."},
	{cmd_insufficient_args, "Incorrect number of arguments supplied for a command."},
	{cmd_invalid, "An invalid command was entered."},
	{cmd_not_found, "No command was entered."},
	{cmd_not_exist, "The specified command does not exist."},
	{not_implemented, "This function is not implemented."},

};

// Displays the actual error message, defined by its code, and then exits the program..
void err(err_code code, std::string extra_msg = "")
{
	print::printline("E" + utils::padLeft(std::to_string(code), '0', err_code_length) + " - " + err_msg[code] + extra_msg);
	exit(EXIT_FAILURE);
}

void error::err_not_implemented(std::string message)
{
	err(not_implemented, print::text_then_quote("Function:", message));
}

void error::err_insufficient_arguments(std::string command)
{
	err(cmd_insufficient_args, print::text_then_quote("Command", command));
}

void error::err_invalid_command(std::string command)
{
	err(cmd_invalid, " Command " + print::quote(command));
}

void error::err_flag_not_exist(std::string flag, bool from_config)
{
	std::string flag_msg = " Flag: " + print::quote(flag);
	
	if(from_config)
	{
		err(flag_not_exist_cfg, flag_msg);
	}
	else
	{
		err(flag_not_exist, flag_msg);
	}
}

void error::err_flag_invalid_arg(std::string flag, std::string arg, bool from_config)
{
	std::string msg = "Flag " + print::quote(flag) + " with argument " + print::quote(arg);
	if(from_config)
	{
		err(flag_invalid_arg_cfg, msg);
	}
	else
	{
		err(flag_invalid_arg, msg);
	}
}

void error::err_cmd_not_found()
{
	err(cmd_not_exist);
}

void error::err_cmd_not_exist(std::string command)
{
	err(cmd_not_found, print::text_then_quote("Command", command));
}