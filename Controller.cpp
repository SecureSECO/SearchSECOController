/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <regex>
#include "Parser.h"
#include "Commands.h"
#include "Print.h"
#include "Utils.h"
#include "Input.h"

int main(int argc, char* argv[])
{
	Input userInput(argc, argv);

	if (Commands::isCommand(userInput.command))
	{
		Commands::execute(userInput.command, userInput.flags);
	}
	else error::err_cmd_not_exist(userInput.command);
	
}
