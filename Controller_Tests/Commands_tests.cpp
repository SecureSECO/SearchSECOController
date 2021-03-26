/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#include "pch.h"
#include "../Commands.h"
#include "../Commands.cpp"
#include "../parser/Parser/parser.cpp"
#include "../parser/Parser/XmlParser.h"
#include "../parser/Parser/SrcMLCaller.h"
#include "../parser/Parser/StringStream.h"
#include "../parser/Parser/Tag.h"
#include "../parser/Parser/Node.h"
#include "../parser/Parser/md5.h"
#include "../parser/Parser/AbstractSyntaxToHashable.h"
#include "../parser/Parser/XmlParser.cpp"
#include "../parser/Parser/SrcMLCaller.cpp"
#include "../parser/Parser/StringStream.cpp"
#include "../parser/Parser/Tag.cpp"
#include "../parser/Parser/Node.cpp"
#include "../parser/Parser/md5.cpp"
#include "../parser/Parser/AbstractSyntaxToHashable.cpp"
TEST(Commands_tests, check_valid_command)
{
	EXPECT_TRUE(Commands::isCommand("start"));
	EXPECT_TRUE(Commands::isCommand("check"));
	EXPECT_TRUE(Commands::isCommand("upload"));
	EXPECT_TRUE(Commands::isCommand("checkupload"));
	EXPECT_TRUE(Commands::isCommand("update"));
	EXPECT_TRUE(Commands::isCommand("version"));
}

TEST(Commands_tests, check_invalid_command)
{
	EXPECT_FALSE(Commands::isCommand("s"));
	EXPECT_FALSE(Commands::isCommand("v"));
	EXPECT_FALSE(Commands::isCommand("console"));
	EXPECT_FALSE(Commands::isCommand("consasdfasasdf asfqaes ole"));
}
