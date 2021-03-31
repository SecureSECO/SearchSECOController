#pragma once

#include <string>
#include <map>

#include "Utils.h"
#include "Flags.h"

class Input
{
public:

	Input(int argc, char* argv[]);

	std::string command;

	std::string executablePath;

	Flags flags;

private:

	void parseCliInput(int argc, char* argv[]);
	void parseExecutablePath(std::string fullPath);
	void parseOptionals(std::string flargs);

	void applyDefaults();

	void sanitizeArguments();

	std::map<std::string, std::string> optionalArguments;
	std::map<std::string, std::string> flagSource;
};

