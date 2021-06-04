/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file.
#include "pch.h"

// Controller includes. 
#include "databaseAPIMock.h"
#include "entrypoint.h"
#include "systemTestsUtils.h"
#include "utils.h"


TEST(systemTestCheck, dummy) 
{
	systemTestsUtils::startAPIMock();

	std::string command = "searchseco check https://github.com/zavg/linux-0.01";
	std::vector<std::string> words = utils::split(command, ' ');

	auto n_argv = systemTestsUtils::getArgv(words);//new char*[4] { "path", "check", "https://github.com/zavg/linux-0.01", nullptr };

	EXPECT_EQ(0, entrypoint::entrypoint(words.size(), n_argv, LOCALHOST, PORT));
}


