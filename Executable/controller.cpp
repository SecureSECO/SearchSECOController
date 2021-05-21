/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "commandFactory.h"
#include "input.h"
#include "parser2.h"
#include "print.h"

// External includes
#include "loguru/loguru.hpp"
#include <functional>
#include <iostream>
#include <map>
#include <string>


int main(int argc, char* argv[])
{
	loguru::g_stderr_verbosity = loguru::Verbosity_OFF;

	loguru::init(argc, argv, { nullptr, "controller" });
	loguru::add_file("logs/searchseco_all.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::add_file("logs/searchseco_low_verbosity.log", loguru::Truncate, loguru::Verbosity_WARNING);

	loguru::g_stderr_verbosity = loguru::Verbosity_WARNING;

	Input userInput(argc, argv);
	auto flags = userInput.flags;

	loguru::g_stderr_verbosity = flags.flag_verbose;
	print::debug("Parsed and sanitized the user input", __FILE__, __LINE__);

	auto commandFactory = new CommandFactory();

	if (flags.flag_help) 
	{
		commandFactory->printHelpMessage(userInput.command);
	}
	else if (flags.flag_version) 
	{
		print::versionFull();
	}
	else
	{
		auto command = commandFactory->getCommand(userInput.command);
		command->execute(flags);
	}

	// Prevent loguru from logging "atexit" to stderr. This is not nice when we want to display the version for example,
	//	it is only visual clutter.
	loguru::g_stderr_verbosity = loguru::Verbosity_OFF;
}