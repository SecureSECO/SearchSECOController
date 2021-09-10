/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "commandFactory.h"
#include "entrypoint.h"
#include "input.h"
#include "print.h"
#include "termination.h"


void entrypoint::entrypoint(int argc, char* argv[], std::string apiIP, std::string apiPort)
{
	// Loguru setup.
	loguru::g_stderr_verbosity = loguru::Verbosity_OFF;

	loguru::init(argc, argv, { nullptr, "controller" });
	loguru::add_file("logs/searchseco_all.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::add_file("logs/searchseco_low_verbosity.log", loguru::Truncate, loguru::Verbosity_WARNING);

	loguru::g_stderr_verbosity = loguru::Verbosity_WARNING;

	// Get user input.
	Input userInput(argc, argv);
	auto flags = userInput.flags;

	loguru::g_stderr_verbosity = flags.flag_verbose;
	print::debug("Parsed and sanitized the user input", __FILE__, __LINE__);

	// Execute command.
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
		auto env = new EnvironmentDTO(apiIP, apiPort, userInput.command, flags.flag_worker_name);

		auto command = commandFactory->getCommand(userInput.command);
		command->execute(flags, env);
	}

	termination::successful();
}
