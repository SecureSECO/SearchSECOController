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


TEST(systemTests, checkuploadSuccesscase)
{
	systemTestsUtils::startAPIMock();

	std::string command = "searchseco checkupload https://github.com/zavg/linux-0.01";
	std::vector<std::string> words = utils::split(command, ' ');

	auto n_argv = systemTestsUtils::getArgv(words);

	ASSERT_EXIT(entrypoint::entrypoint(words.size(), n_argv, LOCALHOST, PORT),
		::testing::ExitedWithCode(EXIT_SUCCESS), ".*"
	);
}
