#include "Print.h"
#include "Utils.h"

#include<iostream>
#include<map>
#include<stdlib.h>

// TODO File versioning
#define CONTROLLER_VERSION "0.0.1"

void print::printline(std::string str)
{
	std::cout << str << '\n';
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
	// TODO File versioning and subsystem versioning
	print::printline("searchseco version " + std::string(CONTROLLER_VERSION));
}

// Logging and warning

void error::log(std::string str)
{
	print::printline("L - " + str);
}

void error::warn(int code)
{
	print::printline("W" + code);
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
};

// Displays the actual error message, defined by its code, and then exits the program..
void err(err_code code, std::string extra_msg = "")
{
	print::printline("E" + utils::padLeft(std::to_string(code), '0', err_code_length) + " - " + err_msg[code] + extra_msg);
	exit(EXIT_FAILURE);
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