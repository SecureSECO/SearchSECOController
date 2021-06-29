/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file.
#include "pch.h"

// Controller includes.
#include "entrypoint.h"
#include "moduleFacades.h"
#include "systemTestsUtils.h"
#include "utils.h"

TEST(systemTests, check______Successcase)
{
	systemTestsUtils::startAPIMock();

	std::string command = "searchseco check https://github.com/zavg/linux-0.01";
	std::vector<std::string> words = Utils::split(command, ' ');

	auto n_argv = systemTestsUtils::getArgv(words);

	systemTestsUtils::resetLogFiles();
	
	ASSERT_EXIT(entrypoint::entrypoint(words.size(), n_argv, LOCALHOST, PORT),
		::testing::ExitedWithCode(EXIT_SUCCESS), ".*"
	);

	std::vector<std::string> logLines = systemTestsUtils::readAllLogLines();
	
	EXPECT_FALSE(systemTestsUtils::crawlerCalled(logLines));
	EXPECT_TRUE(systemTestsUtils::spiderCalled(logLines));
	EXPECT_TRUE(systemTestsUtils::parserCalled(logLines));
}



