#pragma once

#include<string>

namespace print
{
	void printline(std::string str);

	void help();
	void help(std::string str);
}

namespace error 
{
	void log(std::string str);
	void warn(int code);

	void err_insufficient_arguments(std::string command);
	void err_invalid_command(std::string command);
	void err_flag_not_exist(std::string flag, bool from_config);
	void err_flag_invalid_arg(std::string flag, std::string arg, bool from_config);
}
