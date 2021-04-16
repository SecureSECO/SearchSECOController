/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"

#include "../src/loguru/loguru.cpp"

#include "Print.h"
#include "Utils.h"
#include "Error.h"

#include <regex>

#define GENERIC_STRING "teststring"
#define GENERIC_INT 1

// testing data
int test_strc = 5;
std::string* test_strs = new std::string[test_strc]
{
	"test one",
	"my test",
	"testing a function",
	"this is a random string that is very long, look at me I am a string",
	""
};

// test helper functions
bool isSubstring(std::string s1, std::string s2)
{
	return s2.find(s1) != std::string::npos;
}

// print helper functions
TEST(tab_tests, single)
{
	std::string s = "\t";
	std::string tabstring = print::tab(1);
	EXPECT_EQ(s, tabstring);
}

TEST(tab_tests, multiple)
{
	std::string s = "\t\t\t";
	std::string tabstring = print::tab(3);
	EXPECT_EQ(s, tabstring);
}

TEST(encapsulate, succeeding)
{
	for (int i = 0; i < test_strc; ++i)
	{
		std::string word = test_strs[i];
		char c = (char)(std::rand() % 255 + 1);
		std::string encapsulated = print::encapsulate(word, c);

		EXPECT_EQ(c + word + c, encapsulated);
		EXPECT_TRUE(isSubstring(word, encapsulated));
	}
}

TEST(encapsulate, failing)
{
	for (int i = 0; i < test_strc; ++i)
	{
		std::string word = test_strs[i];
		char c = (char)(std::rand() % 255 + 1);
		std::string encapsulated = print::encapsulate(word, c);

		EXPECT_FALSE(word == encapsulated);
		EXPECT_FALSE(std::to_string(c) == encapsulated);
		EXPECT_TRUE(isSubstring(word, encapsulated));
	}
}

TEST(quote, succeeding)
{
	for (int i = 0; i < test_strc; ++i)
	{
		std::string word = test_strs[i];
		char c = '\"';
		std::string encapsulated = print::encapsulate(word, c);

		EXPECT_EQ(c + word + c, encapsulated);
		EXPECT_TRUE(isSubstring(word, encapsulated));
	}
}

TEST(quote, failing)
{
	for (int i = 0; i < test_strc; ++i)
	{
		std::string word = test_strs[i];
		char c = '\'';
		std::string encapsulated = print::encapsulate(word, c);

		EXPECT_FALSE(word == encapsulated);
		EXPECT_FALSE(std::to_string(c) == encapsulated);
		EXPECT_TRUE(isSubstring(word, encapsulated));
	}
}

TEST(plural, singular)
{
	for (int i = 0; i < test_strc; ++i)
	{
		std::string word = test_strs[i];
		std::string pluraled = print::plural(word,1);

		EXPECT_EQ(word, pluraled);
		EXPECT_TRUE(isSubstring(word, pluraled));
	}
}

TEST(plural, plural)
{
	for (int i = 0; i < test_strc; ++i)
	{
		std::string word = test_strs[i];
		char c = 's';
		int n= std::rand() % 200 + 2;
		std::string pluraled = print::plural(word, n);

		EXPECT_EQ(word + c, pluraled);
		EXPECT_TRUE(isSubstring(word, pluraled));
	}
}

TEST(plural, singular_failing)
{
	for (int i = 0; i < test_strc; ++i)
	{
		std::string word = test_strs[i];
		char c = 's';
		std::string pluraled = print::plural(word, 1);

		EXPECT_FALSE(word + c == pluraled);
		EXPECT_TRUE(isSubstring(word, pluraled));
	}
}

TEST(plural, plural_failing)
{
	for (int i = 0; i < test_strc; ++i)
	{
		std::string word = test_strs[i];
		int n = std::rand() % 200 + 2;
		std::string pluraled = print::plural(word, n);

		EXPECT_FALSE(word == pluraled);
		EXPECT_TRUE(isSubstring(word, pluraled));
	}
}

// printing functions

TEST(Print_line, print_line_test)
{
	for (int i = 0; i < test_strc; ++i)
	{
		testing::internal::CaptureStdout();
		std::string input = test_strs[i];
		print::printline(input);
		std::string output = testing::internal::GetCapturedStdout();
		EXPECT_EQ(output, input + '\n');
	}

}

TEST(version_test, version_regex)
{
	testing::internal::CaptureStdout();

	print::version_full();

	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE(std::regex_match(output, std::regex("((searchseco|parser|spider|database_api) version \\d*.\\d*.\\d*\\n(\\t)?)*")));
}

// ERROR TESTING

int test_intc = 6;
int* test_ints = new int[test_intc]
{
	INT_MIN,
	-1,
	0,
	1,
	5,
	INT_MAX
};

// All error throwing functions should have more or less the same behaviour: print some string to stdout,
// and then kill the program.
// It seems to be impossible to test whether this string is in the correct format, since the program terminates
// before we are able to read stdout, so for now these death tests only assert that all error functions terminate
// the program with code EXIT_FAILURE.

TEST(error_death_tests, err_insufficient_arguments)
{
	ASSERT_EXIT(error::err_cmd_incorrect_arguments(GENERIC_STRING, 1,2, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_flag_not_exist)
{
	ASSERT_EXIT(error::err_flag_not_exist(GENERIC_STRING, true, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
	ASSERT_EXIT(error::err_flag_not_exist(GENERIC_STRING, false, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_flag_invalid_arg)
{
	ASSERT_EXIT(error::err_flag_invalid_arg(GENERIC_STRING, GENERIC_STRING, true, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
	ASSERT_EXIT(error::err_flag_invalid_arg(GENERIC_STRING, GENERIC_STRING, false, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_cmd_not_found)
{
	ASSERT_EXIT(error::err_cmd_not_found(__FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_cmd_not_exist)
{
	ASSERT_EXIT(error::err_cmd_not_exist(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_not_implemented)
{
	ASSERT_EXIT(error::err_not_implemented(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}
