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
TEST(tab, single)
{
	std::string s = "\t";
	std::string tabstring = print::tab(1);
	EXPECT_EQ(s, tabstring);
}

TEST(tab, multiple)
{
	std::string s = "\t\t\t";
	std::string tabstring = print::tab(3);
	EXPECT_EQ(s, tabstring);
}

TEST(encapsulate, successcase)
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

TEST(encapsulate, failurecase)
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

TEST(quote, successcase)
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

TEST(quote, failurecase)
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

TEST(plural, singuilarFailurecase)
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

TEST(plural, pluralFailurecase)
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

TEST(printline, successcase)
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

TEST(version, versionRegex)
{
	testing::internal::CaptureStdout();

	print::versionFull();

	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE(std::regex_match(output, std::regex("((>> )?(searchseco|parser|spider|crawler) version\\s?(\\d*.\\d*.\\d*)?\\n)*")));
}

// Matches Printing.

// Dummy variables.
std::vector<HashData> dummyHashes = { HashData("hash1", "func1", "file1", 4, 20), HashData("hash2", "func2", "file2", 6, 9) };
std::string dummyDatabaseOutput = "hash2?1?2?func3?file3?10?1?5\n";

AuthorData getDummyAuthorData() 
{
	AuthorData authordata;
	CommitData cd1 = CommitData();
	cd1.author = "Author";
	cd1.authorMail = "author@mail.com";
	CommitData cd2 = CommitData();
	cd2.author = "Author2";
	cd2.authorMail = "author2@mail.com";
	CommitData cd3 = CommitData();
	cd3.author = "Author3";
	cd3.authorMail = "author3@mail.com";

	CodeBlock cb1 = CodeBlock();
	cb1.commit = std::make_shared<CommitData>(cd1);
	cb1.line = 0;
	cb1.numLines = 2;
	authordata["file1"].push_back(cb1);

	CodeBlock cb2 = CodeBlock();
	cb2.commit = std::make_shared<CommitData>(cd2);
	cb2.line = 5;
	cb2.numLines = 8;
	authordata["file2"].push_back(cb2);

	CodeBlock cb3 = CodeBlock();
	cb3.commit = std::make_shared<CommitData>(cd3);
	cb3.line = 11;
	cb3.numLines = 20;
	authordata["file2"].push_back(cb3);

	return authordata;
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

TEST(errorDeathTests, errCmdIncorrectArguments)
{
	ASSERT_EXIT(error::errCmdIncorrectArguments(GENERIC_STRING, 1,2, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errFlagNotExist)
{
	ASSERT_EXIT(error::errFlagNotExist(GENERIC_STRING, true, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
	ASSERT_EXIT(error::errFlagNotExist(GENERIC_STRING, false, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errFlagInvalidArg)
{
	ASSERT_EXIT(error::errFlagInvalidArg(GENERIC_STRING, GENERIC_STRING, true, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
	ASSERT_EXIT(error::errFlagInvalidArg(GENERIC_STRING, GENERIC_STRING, false, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errCmdNotFound)
{
	ASSERT_EXIT(error::errCmdNotFound(__FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errCmdNotExist)
{
	ASSERT_EXIT(error::errCmdNotExist(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errParseCallSyntaxError)
{
	ASSERT_EXIT(error::errParseCallSyntaxError(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errParseIncorrectShorthandFlag)
{
	ASSERT_EXIT(error::errParseIncorrectShorthandFlag(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errParseIncorrectLonghandFlag)
{
	ASSERT_EXIT(error::errParseIncorrectLonghandFlag(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errParseCouldNotParseFlag)
{
	ASSERT_EXIT(error::errParseCouldNotParseFlag(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errInvalidUrl)
{
	ASSERT_EXIT(error::errInvalidUrl(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(errorDeathTests, errNotImplemented)
{
	ASSERT_EXIT(error::errNotImplemented(GENERIC_STRING, __FILE__, __LINE__), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

