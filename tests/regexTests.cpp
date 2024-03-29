/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file.
#include "pch.h"

// Controller includes.
#include "regexWrapper.h"

// External includes.
#include <map>


#pragma region Cross-Test variable declarations

int validURLc = 4;
std::string *validURLs = new std::string[validURLc]{
	"http://github.com/UserName/project", 
	"https://www.github.com/AnotherUser/project-with-hyphens",
	"www.github.com/username-with-hyphens/CoolProject",
	"github.com/short-url/but-long-hyphenated-project-containing-many-potential-files_which-we"
		"-would-like-to-spider-and-parse"};

#pragma endregion

#pragma region Syntax Regex Tests

TEST(regex, syntaxNoURLSuccesscase)
{
	// Arrange.
	std::map<
		std::string, 
		std::tuple<
			std::string, 
			std::string, 
			std::string>> 
		testcases = 
	{
		{"start --cpu 8 -o console", std::make_tuple("start", "", "--cpu 8 -o console")},
		{"-h", std::make_tuple("", "", "-h")},
		{"-v", std::make_tuple("", "", "-v")},
		{"upload -h", std::make_tuple("upload", "", "-h")},
		{"argumentOutOfRangeButAccepted -V 15", std::make_tuple("argumentOutOfRangeButAccepted", "", "-V 15")},
		{"unnessecaryWhiteSpacesIgnored    -h      ", std::make_tuple("unnessecaryWhiteSpacesIgnored", "", "-h")},
		{"nonexistantButAcceptedCommand -V 3 --flag argument",
			std::make_tuple("nonexistantButAcceptedCommand", "", "-V 3 --flag argument")},
		{"commandButNoFlags", std::make_tuple("commandButNoFlags", "", "")}
	};

	// Act.
	for (auto const& testcase : testcases)
	{
		auto input = testcase.first;
		auto expectedOutput = testcase.second;

		std::tuple<std::string, std::string, std::string> output;

		// Assert.

		ASSERT_TRUE(regex::validateSyntax(input, output));

		EXPECT_EQ(output, expectedOutput);
	}
}

TEST(regex, syntax__URLSuccesscase)
{
	// Arrange.
	std::map<
		std::string, 
		std::tuple<
			std::string, 
			std::string,
			std::string>> 
		testcases = 
	{
		{"command " + validURLs[0] + " --flag argument", 
			std::make_tuple("command", validURLs[0], "--flag argument")},
		{"command " + validURLs[1], 
			std::make_tuple("command", validURLs[1], "")},
		{"command " + validURLs[2] + " --output console", 
			std::make_tuple("command", validURLs[2], "--output console")},
		{"command " + validURLs[3] + " -V 3", 
			std::make_tuple("command", validURLs[3], "-V 3")},
	};

	// Act.
	for (auto const& testcase : testcases)
	{
		auto input = testcase.first;
		auto expectedOutput = testcase.second;

		std::tuple<std::string, std::string, std::string> output;

		// Assert.
		
		ASSERT_TRUE(regex::validateSyntax(input, output));

		EXPECT_EQ(output, expectedOutput);
	}
}

TEST(regex, syntaxNoURLFailurecase)
{
	// Arrange.
	int testcasec = 1;
	std::string *testcases = new std::string[testcasec]
	{
		""
	};

	// Act.
	for (int i = 0; i < testcasec; ++i)
	{
		auto input = testcases[i];

		std::tuple<std::string, std::string, std::string> output;

		// Assert.

		EXPECT_FALSE(regex::validateSyntax(input, output));
	}
}
#pragma endregion

#pragma region Flag Argument Pair Regex Tests

TEST(regex, parseFlargsSuccesscase)
{
	// Arrange.
	std::map<std::string,       // Input.
			 std::map<          // Map of flag-argument pairs:
				std::string,	//	key   = flag.
				std::string>	//	value = argument.
			 >
		testcases = 
	{
		{
			"-V 3 --output console", 
			{
				{"V", "3"},
				{"output", "console"}
			}
		},
		{
			"--cpu 8 --looksLikeAFlagButDoesNotExist itShouldStillBeParsedThough", 
			{
				{"cpu", "8"},
				{"looksLikeAFlagButDoesNotExist", "itShouldStillBeParsedThough"},
			}
		},
		{
			"-v", 
			{
				{"v", ""}
			}
		}
	};

	for (auto const& testcase : testcases)
	{
		auto input = testcase.first;
		
		auto expected_output = testcase.second;

		std::map<std::string, std::string> output;

		// Act & Assert.
		
		ASSERT_NO_FATAL_FAILURE(regex::parseFlargPairs(input, output));

		ASSERT_EQ(output.size(), expected_output.size());

		for (auto const& pair : output)
		{
			auto key = pair.first;

			EXPECT_EQ(output[key], expected_output[key]);  
		}
	}
}

TEST(regex, parseFlargsFailurecase___fatal)
{
	// Arrange.
	int testcasec = 3;
	std::string *testcases = new std::string[testcasec]
	{
		"-singleHyphenMultiChar", 
		"--d oubleHyphenSingleChar", 
		"---moreThanTwoHyphens"
	};

	std::map<std::string, std::string> output;

	ASSERT_EXIT(regex::parseFlargPairs(testcases[0], output), ::testing::ExitedWithCode(12), ".*");
	ASSERT_EXIT(regex::parseFlargPairs(testcases[1], output), ::testing::ExitedWithCode(11), ".*");
	ASSERT_EXIT(regex::parseFlargPairs(testcases[2], output), ::testing::ExitedWithCode(13), ".*");
	
}

TEST(regex, parseFlargsFailurecaseNonfatal)
{
	// Arrange.
	int testcasec = 3;
	std::map<
		std::string,		// Input.
		std::map<			// Expected Output.
			std::string,	//	Expected Flag.
			std::string>	//	Expected Argument.
		> testcases = 
	{
		{
			"--flag with twoArguments",
			{
				{"flag", "with"}
			}
		},
		{
			"flagLessArgument",
			{}
		},
		{
			"--correctFlag withAnArgument --combined with aMultipleArgumentFlag",
			{
				{"correctFlag", "withAnArgument"},
				{"combined", "with"}
			}
		}
		
	};

	std::map<std::string, std::string> output;

	for (auto const& testcase : testcases)
	{
		auto input = testcase.first;
		auto expected_output = testcase.second;

		// Act.
		regex::parseFlargPairs(input, output);

		// Assert.
		ASSERT_EQ(output.size(), expected_output.size());
		
		for (auto const& pair : output)
		{
			auto key = pair.first;

			EXPECT_EQ(output[key], expected_output[key]);
		}
	}
}

#pragma endregion

#pragma region URL Validation Tests

TEST(regex, urlValidationSuccesscase)
{
	// Arrange has been taken care of.

	// Act & Assert.

	for (int i = 0; i < validURLc; ++i)
	{
		auto input = validURLs[i];

		EXPECT_TRUE(regex::validateURL(input));    
	}
}

TEST(regex, urlValidationFailurecase)
{
	// Arrange.
	int invalidURLc = 5;
	std::string *invalidURLs = new std::string[invalidURLc]
	{
		"notAURL", 
		"completely wrong string", 
		"ptth//:this_is_not_a.valid/url", 
		"www.-github.com/no-hyphens-in-front",
		"www.github"
	};

	// Act & Assert.

	for (int i = 0; i < invalidURLc; ++i)
	{
		auto input = invalidURLs[i];

		EXPECT_FALSE(regex::validateURL(input));
	}
}

#pragma endregion
