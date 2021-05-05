/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "commands.h"


TEST(Commands_tests, check_valid_command)
{
	EXPECT_TRUE(Commands::isCommand("start"));
	EXPECT_TRUE(Commands::isCommand("check"));
	EXPECT_TRUE(Commands::isCommand("upload"));
	EXPECT_TRUE(Commands::isCommand("checkupload"));
	EXPECT_TRUE(Commands::isCommand("update"));
}

TEST(Commands_tests, check_invalid_command)
{
	EXPECT_FALSE(Commands::isCommand("s"));
	EXPECT_FALSE(Commands::isCommand("v"));
	EXPECT_FALSE(Commands::isCommand("console"));
	EXPECT_FALSE(Commands::isCommand("consasdfasasdf asfqaes ole"));
}
