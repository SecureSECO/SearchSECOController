/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file.
#include "pch.h"

// Controller includes. 
#include "entrypoint.h"
#include "databaseAPIMock.h"




TEST(systemTestCheck, dummy) 
{
	std::string command = "searchseco check https://github.com/zavg/linux-0.01";

	//char* n_argv[] = { "path", "searchseco", "check", "https://github.com/zavg/linux-0.01" };

	EXPECT_NO_THROW(entrypoint::dummy());
	//EXPECT_NO_THROW(controller::entryPoint(4, n_argv));
}


