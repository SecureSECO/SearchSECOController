#pragma once

#include <map>
#include <string>
#include <functional>

class Commands
{
public:
	static void execute(std::string s, std::map<std::string, std::string> flags);
	static bool isCommand(std::string s);
private:
	static std::map<std::string, std::function<void(std::map<std::string, std::string>)>> perform;
};
