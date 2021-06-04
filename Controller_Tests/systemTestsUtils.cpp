/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file.
#include "pch.h"

// Controller includes. 
#include "databaseAPIMock.h"
#include "systemTestsUtils.h"
#include "utils.h"


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
