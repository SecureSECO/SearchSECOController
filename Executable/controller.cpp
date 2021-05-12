/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "Commands.h"
#include "Input.h"
#include "Parser2.h"
#include "Print.h"

// External includes
#include "loguru/loguru.hpp"
#include <functional>
#include <iostream>
#include <map>
#include <string>


#include <thread>


int main(int argc, char* argv[])
{
	int x = std::thread::hardware_concurrency();

	print::printline(std::to_string(x));
	loguru::g_stderr_verbosity = loguru::Verbosity_OFF;

	loguru::init(argc, argv, { nullptr, "controller" });
	loguru::add_file("logs/searchseco_all.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::add_file("logs/searchseco_low_verbosity.log", loguru::Truncate, loguru::Verbosity_WARNING);

	loguru::g_stderr_verbosity = loguru::Verbosity_WARNING;

	Input userInput(argc, argv);

	loguru::g_stderr_verbosity = userInput.flags.flag_verbose;
	print::debug("Parsed and sanitized the user input", __FILE__, __LINE__);

	Commands::execute(userInput.command, userInput.flags);

	// Prevent loguru from logging "atexit" to stderr. This is not nice when we want to display the version for example,
	//	it is only visual clutter.
	loguru::g_stderr_verbosity = loguru::Verbosity_OFF;
}
