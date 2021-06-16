/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file.
#include "pch.h"

// Controller includes. 
#include "databaseAPIMock.h"
#include "entryPoint.h"
#include "systemTestsUtils.h"
#include "utils.h"

// External includes.
#include <iostream>
#include <fstream>


void systemTestsUtils::startAPIMock() 
{
	ConnectionHandler *connectionHandler = new ConnectionHandler();
    std::thread *t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);
}


char** systemTestsUtils::getArgv(std::vector<std::string> &words, std::string path) 
{
	auto n_argv = new char* [words.size() + 1] {};

	n_argv[0] = &(path[0]);
	for (int i = 1; i < words.size(); i++) 
	{
		n_argv[i] = &(words[i][0]);
	}
	n_argv[words.size()] = nullptr;

	return n_argv;
}

void systemTestsUtils::resetLogFiles() 
{
	remove("logs/searchseco_all.log");
	remove("logs/searchseco_low_verbosity.log");
}

std::vector<std::string> systemTestsUtils::readAllLogLines() 
{
	std::string logs;

	std::ifstream ReadLogFile("logs/searchseco_all.log");
	if (!ReadLogFile.is_open()) 
	{
		print::printline("Unable to open log file");
	}

	while (std::getline(ReadLogFile, logs))
	{
		continue;
	}

	return utils::split(logs, '\n');
}

bool componentCalled(std::string component, std::vector<std::string> lines) 
{
	std::string line;
	std::vector<std::string> words;
	for (int i = 0; i < lines.size(); i++)
	{
		line = lines[i];
		utils::replace(line, ']', '[');
		words = utils::split(line, '[');

		for (int j = 0; j < words.size(); j++) 
		{
			std::string word = utils::trimWhiteSpaces(words[j]);
			if (word == component) 
			{
				return true;
			}
		}
	}

	return false;
}

bool systemTestsUtils::crawlerCalled(std::vector<std::string> outputLines)
{
	return componentCalled("crawler", outputLines);
}

bool systemTestsUtils::spiderCalled(std::vector<std::string> outputLines)
{
	return componentCalled("spider", outputLines);
}

bool systemTestsUtils::parserCalled(std::vector<std::string> outputLines)
{
	return componentCalled("parser", outputLines);
}
