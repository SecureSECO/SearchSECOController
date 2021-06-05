/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file.
#include "pch.h"

// Controller includes.
#include "entrypoint.h"
#include "systemTestsUtils.h"
#include "utils.h"


TEST(systemTestVersion, dummy) 
{
	std::string command = "searchseco -v";
	std::vector<std::string> words = utils::split(command, ' ');

	auto n_argv = systemTestsUtils::getArgv(words);

	EXPECT_EQ(0, entrypoint::entrypoint(words.size(), n_argv, LOCALHOST, PORT));
}


