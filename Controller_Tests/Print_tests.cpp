/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"
#include "../Print.h"
#include "../Print.cpp"
#include "../Utils.cpp"


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
	}
}

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

TEST(error_testing, log)
{
	std::string output;
	for (int i = 0; i < test_strc; ++i)
	{
		testing::internal::CaptureStdout();

		error::log(test_strs[i]);
		output = testing::internal::GetCapturedStdout();

		ASSERT_EQ("L - " + test_strs[i] + '\n', output);
	}
}

TEST(error_testing, warn)
{
	std::string output;
	for (int i = 0; i < test_intc; ++i)
	{
		int code = test_ints[i];

		if (code <= 0)
		{
			ASSERT_THROW(error::warn(code), std::out_of_range);
		}
		else
		{
			testing::internal::CaptureStdout();

			error::warn(code);

			output = testing::internal::GetCapturedStdout();

			output = utils::split(output, '-')[0];
			ASSERT_EQ("W" + std::to_string(test_ints[i]) + ' ', output);
		}
	}
}

TEST(error_testing, err_insufficient_arguments)
{
	SUCCEED();
}

TEST(error_testing, err_invalid_command)
{
	SUCCEED();
}

TEST(error_testing, err_flag_not_exist)
{
	SUCCEED();
}

TEST(error_testing, err_flag_invalid_arg)
{
	SUCCEED();
}

TEST(error_testing, err_cmd_not_found)
{
	SUCCEED();
}

TEST(error_testing, err_cmd_not_exist)
{
	SUCCEED();
}

TEST(error_testing, err_not_implemented)
{
	SUCCEED();
}
