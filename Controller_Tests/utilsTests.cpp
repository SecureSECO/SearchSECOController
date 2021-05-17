/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "utils.h"


TEST(contains, isInVector)
{
	std::vector<int> v = { 1, 2, 3, 4 };
	EXPECT_TRUE(utils::contains(v, 1));
	EXPECT_FALSE(utils::contains(v, 5));
}

TEST(contains, isInArray)
{
	const char* c = "string";
	EXPECT_TRUE(utils::contains(c, 's', 6));
	EXPECT_FALSE(utils::contains(c, 'x', 6));
}

TEST(split, basicSplit)
{
	std::string s = "Hello World!";
	std::vector<std::string> splitted = utils::split(s, ' ');
	EXPECT_EQ("Hello", splitted[0]);
	EXPECT_EQ("World!", splitted[1]);
}

TEST(split, wrongSplit)
{
	std::string s = "Hello World!";
	std::vector<std::string> splitted = utils::split(s, 'x');
	EXPECT_EQ("Hello World!", splitted[0]);
}

TEST(split, multipleSplit)
{
	std::string s = "Hello World!";
	std::vector<std::string> splitted = utils::split(s, 'l');
	EXPECT_EQ("He", splitted[0]);
	EXPECT_EQ("", splitted[1]);
	EXPECT_EQ("o Wor", splitted[2]);
	EXPECT_EQ("d!", splitted[3]);
}

TEST(trim, basicTrim)
{
	std::string s = "  spaces  ";
	EXPECT_EQ("spaces", utils::trim(s, " "));
}

TEST(trim, delimiterInMiddleTrim)
{
	std::string s = "spa ces  ";
	EXPECT_EQ("spa ces", utils::trim(s, " "));
}

TEST(trim, whitespaceTrim)
{
	std::string s = " \n\r\ttest \n\t";
	EXPECT_EQ("test", utils::trimWhiteSpaces(s));
}

TEST(isNumber, successcase)
{
	EXPECT_TRUE(utils::isNumber("123"));
}

TEST(isNumber, failurecase)
{
	EXPECT_FALSE(utils::isNumber("1d3"));
}

TEST(padLeft, basicPadLeft)
{
	EXPECT_EQ("   test", utils::padLeft("test", ' ', 7));
}

TEST(replace, basicReplace)
{
	std::string str = "hallo";
	utils::replace(str, 'a', 'e');
	EXPECT_EQ(str, "hello");
}

TEST(replace, nothingToReplace)
{
	std::string str = "hallo";
	utils::replace(str, 'w', 'e');
	EXPECT_EQ(str, "hallo");
}
