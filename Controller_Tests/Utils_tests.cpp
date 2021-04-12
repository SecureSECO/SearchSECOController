/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"
#include "Utils.h"


TEST(contains_tests, is_in_vector)
{
	std::vector<int> v = { 1, 2, 3, 4 };
	EXPECT_TRUE(utils::contains(v, 1));
	EXPECT_FALSE(utils::contains(v, 5));
}

TEST(contains_tests, is_in_array)
{
	const char* c = "string";
	EXPECT_TRUE(utils::contains(c, 's', 6));
	EXPECT_FALSE(utils::contains(c, 'x', 6));
}

TEST(split_tests, basic_split)
{
	std::string s = "Hello World!";
	std::vector<std::string> splitted = utils::split(s, ' ');
	EXPECT_EQ("Hello", splitted[0]);
	EXPECT_EQ("World!", splitted[1]);
}

TEST(split_tests, wrong_split)
{
	std::string s = "Hello World!";
	std::vector<std::string> splitted = utils::split(s, 'x');
	EXPECT_EQ("Hello World!", splitted[0]);
}

TEST(split_tests, multiple_split)
{
	std::string s = "Hello World!";
	std::vector<std::string> splitted = utils::split(s, 'l');
	EXPECT_EQ("He", splitted[0]);
	EXPECT_EQ("", splitted[1]);
	EXPECT_EQ("o Wor", splitted[2]);
	EXPECT_EQ("d!", splitted[3]);
}

TEST(trim_tests, basic_trim)
{
	std::string s = "  spaces  ";
	EXPECT_EQ("spaces", utils::trim(s, " "));
}

TEST(trim_tests, delimiter_in_middle_trim)
{
	std::string s = "spa ces  ";
	EXPECT_EQ("spa ces", utils::trim(s, " "));
}

TEST(trim_tests, whitespace_trim)
{
	std::string s = " \n\r\ttest \n\t";
	EXPECT_EQ("test", utils::trimWhiteSpaces(s));
}

TEST(is_number_tests, number)
{
	EXPECT_TRUE(utils::isNumber("123"));
}

TEST(is_number_tests, not_number)
{
	EXPECT_FALSE(utils::isNumber("1d3"));
}

TEST(pad_left_tests, basic_pad_left)
{
	EXPECT_EQ("   test", utils::padLeft("test", ' ', 7));
}