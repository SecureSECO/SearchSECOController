#include "Print.h"
#include "Utils.h"

#include<iostream>
#include<map>

void printline(std::string str)
{
	std::cout << str << '\n';
}

void help()
{
	printline("help section is yet to be implemented");
}

void help(std::string message)
{
	printline(message);
	help();
}

// ERRORS

int err_code_length = 3;

enum err_code
{
	flag_not_exist = 1,
	flag_not_exist_cfg,
	flag_invalid_arg,
	flag_invalid_arg_cfg
};

std::map <int, std::string> err_msg =
{
	{flag_not_exist, "The specified flag does not exist."},
	{flag_not_exist_cfg, "The specified flag does not exist in the config file."},
	{flag_invalid_arg, "The specified argument is invalid for its corresponding flag."},
	{flag_invalid_arg_cfg, "The specified argument is invalid for its corresponding flag in the config file."},
};

void err(int code, std::string extra_msg = "")
{
	// TODO JOCHEM: throw so program wont go on and hlep
	printline("E" + utils::pad_left(std::to_string(code), '0', err_code_length) + " - " + err_msg[code] + extra_msg);
	throw 123434;
}


void error::log(std::string str)
{
	printline("L - " + str);
}

void error::warn(int code)
{
	printline("W" + code);
}

void error::err_insufficient_arguments(std::string command)
{
	//TODO JOCHEM
	printline("Need a argument for " + command);
}

void error::err_invalid_command(std::string command)
{
	// TODO JOCHEM
	printline("WRONG COMMAND!!!!! " + command);
}

void error::err_flag_not_exist(std::string flag, bool from_config)
{
	std::string flag_msg = "Flag: " + flag;
	
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
	if(from_config)
	{
		err(flag_invalid_arg_cfg, "Flag \"" + flag + "\" with argument \"" + arg + "\"");
	}
	else
	{
		err(flag_invalid_arg, "Flag \"" + flag + "\" with argument \"" + arg + "\"");
	}
}