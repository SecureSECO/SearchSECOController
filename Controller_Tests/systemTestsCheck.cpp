/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file.
#include "pch.h"

// Controller includes. 
#include "entrypoint.h"
#include "databaseAPIMock.h"

// Constants.
#define LOCALHOST "127.0.0.1"
#define PORT "6969"


TEST(systemTestCheck, dummy) 
{
	std::string command = "searchseco check https://github.com/zavg/linux-0.01";

	auto n_argv = new char*[4] { "path", "check", "https://github.com/zavg/linux-0.01", nullptr };

	EXPECT_EQ(0, entrypoint::entrypoint(3, n_argv, LOCALHOST, PORT));
}


