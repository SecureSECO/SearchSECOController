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

namespace 
{
	int err_code_length = 3;

	enum err_code
	{
		flag_not_exist = 1,
		flag_not_exist_cfg,
		flag_invalid_arg
	};

	std::map <int, std::string> err_msg =
	{
		{flag_not_exist, "The specified flag does not exist."},
		{flag_not_exist_cfg, "The specified flag in the config file does not exist."},
		{flag_invalid_arg, "The specified argument is invalid for its corresponding flag."},
	};

	void err(int code, std::string extra_msg = "")
	{
		printline("E" + utils::pad_left(std::to_string(code), '0', err_code_length) + " - " + err_msg[code] + extra_msg);
	}
}

void log(std::string str)
{
	printline("L - " + str);
}

void warn(int code)
{
	printline("W" + code);
}

void err_flag_not_exist(bool from_config, std::string flag)
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

void err_flag_invalid_arg(std::string flag, std::string arg, bool from_config)
{
	err(flag_invalid_arg, "Flag \"" + flag + "\" with argument \"" + arg + "\"");
}