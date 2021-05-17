/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "commandFactory.h"
#include "databaseRequests.h"
#include "print.h"

// Parser includes
#include "Parser.h"

// Spider includes
#include "RunSpider.h"


CommandFactory::CommandFactory()
{
	this->comMap = 
	{
		{"start", new Start()},
		{"check", new Check()},
		{"upload", new Upload()},
		{"checkupload", new CheckUpload()},
		{"update", new Update()}
	};

	this->helpMessageCommonFlags = R"(
	-V --verbose: Sets the verbosity level of the console output. Can be used in combination with any command.
		Arguments:
			The verbosity level. Allowed values are:
				1: Nothing 
				2: Only errors
				3: Warning and errors
				4: Everything (default value)
	-v --version: Displays the version of the SearchSECO system, and the version of each of its subsystems.
	-h --help: Displays the help message. Can be used in conjunction with other flags to specify the message.)";
}

Command* CommandFactory::getCommand(std::string commandString)
{
	if (this->comMap.count(commandString) != 1)
	{
		error::errCmdNotExist(commandString, __FILE__, __LINE__);
	}

	return comMap[commandString];
}

void CommandFactory::printHelpMessage(std::string commandString)
{
	print::printline("For more information about what each command does, check the documentation.");
	if (commandString == "" || this->comMap.count(commandString) == 0)
	{
		print::printline("\nCommands: \n");
		for (auto const& [_, command] : this->comMap)
		{
			print::printline(command->helpMessage());
			print::printline("");
		}
	}
	else
	{
		print::printline(this->comMap[commandString]->helpMessage());
	}
	print::printline("\nCommon flags:");
	print::printline(this->helpMessageCommonFlags);
}
