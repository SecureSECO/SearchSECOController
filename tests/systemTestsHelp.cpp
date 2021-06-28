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

TEST(systemTests, help_______Successcase)
{
	std::string command = "searchseco -h";
	std::vector<std::string> words = Utils::split(command, ' ');

	auto n_argv = systemTestsUtils::getArgv(words);

	systemTestsUtils::resetLogFiles();

	ASSERT_EXIT(entrypoint::entrypoint(words.size(), n_argv, LOCALHOST, PORT), 
		::testing::ExitedWithCode(EXIT_SUCCESS), ".*"
	);

	std::vector<std::string> logLines = systemTestsUtils::readAllLogLines();
	
	EXPECT_FALSE(systemTestsUtils::crawlerCalled(logLines));
	EXPECT_FALSE(systemTestsUtils::spiderCalled(logLines));
	EXPECT_FALSE(systemTestsUtils::parserCalled(logLines));
}
