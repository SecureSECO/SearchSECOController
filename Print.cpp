#include "Print.h"
#include "Utils.h"

#include<iostream>
#include<map>
#include<stdlib.h>

void print::printline(std::string str)
{
	std::cout << str << '\n';
}

void print::help()
{
	printline("help section is yet to be implemented");
}

void print::help(std::string message)
{
	printline(message);
	help();
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
// ERRORS

int err_code_length = 3;

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

void err(int code, std::string extra_msg = "")
{
	print::printline("E" + utils::pad_left(std::to_string(code), '0', err_code_length) + " - " + err_msg[code] + extra_msg);
	exit(EXIT_FAILURE);
}


void error::log(std::string str)
{
	print::printline("L - " + str);
}

void error::warn(int code)
{
	print::printline("W" + code);
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