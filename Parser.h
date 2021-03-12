#pragma once

#include<string>
#include<map>

class Parser
{
public:
	static std::map<std::string, std::string> parse(std::string path, std::string* args, int argc);
private:
	static std::map<std::string, std::string> getDefaultFlagsForCommand(std::string command);
	static void parseFile(std::map<std::string, std::string>& flagArgs, std::string path);
	static void parseFlags(std::map<std::string, std::string>& flagArgs, std::string* args, int argc, int start);
	static void sanitize(std::map<std::string, std::string>& flagArgs, std::string flag, std::string argument, bool fromFile);
};