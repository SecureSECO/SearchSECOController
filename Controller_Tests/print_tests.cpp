/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "error.cpp"
#include "print.h"
#include "utils.h"

// External includes
#include <climits>
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

	print::versionFull();

	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE(std::regex_match(output, std::regex("((>> )?(searchseco|parser|spider|crawler) version\\s?(\\d*.\\d*.\\d*)?\\n)*")));
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
	ASSERT_EXIT(error::errCmdIncorrectArguments(GENERIC_STRING, 1,2, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_flag_not_exist)
{
	ASSERT_EXIT(error::errFlagNotExist(GENERIC_STRING, true, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
	ASSERT_EXIT(error::errFlagNotExist(GENERIC_STRING, false, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_flag_invalid_arg)
{
	ASSERT_EXIT(error::errFlagInvalidArg(GENERIC_STRING, GENERIC_STRING, true, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
	ASSERT_EXIT(error::errFlagInvalidArg(GENERIC_STRING, GENERIC_STRING, false, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_cmd_not_found)
{
	ASSERT_EXIT(error::errCmdNotFound(__FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_cmd_not_exist)
{
	ASSERT_EXIT(error::errCmdNotExist(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_parse_call_syntax_error)
{
	ASSERT_EXIT(error::errParseCallSyntaxError(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_parse_incorrect_shorthand_flag)
{
	ASSERT_EXIT(error::errParseIncorrectShorthandFlag(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_parse_incorrect_longhand_flag)
{
	ASSERT_EXIT(error::errParseIncorrectLonghandFlag(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_parse_could_not_parse_flag)
{
	ASSERT_EXIT(error::errParseCouldNotParseFlag(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_invalid_url)
{
	ASSERT_EXIT(error::errInvalidUrl(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(error_death_tests, err_not_implemented)
{
	ASSERT_EXIT(error::errNotImplemented(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(print_hash_matches, basic_matches)
{
	testing::internal::CaptureStdout();

	std::vector<HashData> hashes = {
		HashData("hash1", "func1", "file1", 1, 2),
		HashData("hash2", "func2", "file2", 4, 6),
	};

	std::string dbout = "hash1?5?0?func10?file10?100?0\n";

	print::printHashMatches(hashes, dbout);

	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "func1 in file file1 line 1 was found in our database: \nFunction func10 in project 5 in file file10 line 100\n\n");
}

TEST(print_hash_matches, basic_double_db_output)
{
	testing::internal::CaptureStdout();

	std::vector<HashData> hashes = {
		HashData("hash1", "func1", "file1", 1, 2),
		HashData("hash2", "func2", "file2", 4, 6),
	};

	std::string dbout = "hash1?5?0?func10?file10?100?0\nhash1?5?0?func10?file10?100?0\n";

	print::printHashMatches(hashes, dbout);

	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "func1 in file file1 line 1 was found in our database: \nFunction func10 in project 5 in file file10 line 100\n\n");
}

TEST(print_hash_matches, basic_double_hash_input)
{
	testing::internal::CaptureStdout();

	std::vector<HashData> hashes = {
		HashData("hash1", "func1", "file1", 1, 2),
		HashData("hash1", "func3", "file1", 1, 2),
		HashData("hash2", "func2", "file2", 4, 6),
	};

	std::string dbout = "hash1?5?0?func10?file10?100?0\n";

	print::printHashMatches(hashes, dbout);

	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "func1 in file file1 line 1 was found in our database: \nFunction func10 in project 5 in file file10 line 100\n\nfunc3 in file file1 line 1 was found in our database: \nFunction func10 in project 5 in file file10 line 100\n\n");
}
