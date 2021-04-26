#include "pch.h"

#include "regex_wrapper.h"

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

TEST(regex_test, syntax_nourl_successcase)
{
	// Arrange
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

	// Act 
    for (auto const& testcase : testcases)
	{
        auto input = testcase.first;
        auto expectedOutput = testcase.second;

		std::tuple<std::string, std::string, std::string> output;

		// Assert

        ASSERT_TRUE(regex::validateSyntax(input, output));

		EXPECT_EQ(output, expectedOutput);
	}
}

TEST(regex_test, syntax___url_successcase)
{
    // Arrange
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

	// Act
    for (auto const& testcase : testcases)
    {
        auto input = testcase.first;
        auto expectedOutput = testcase.second;

        std::tuple<std::string, std::string, std::string> output;

        // Assert
        
		ASSERT_TRUE(regex::validateSyntax(input, output));

		EXPECT_EQ(output, expectedOutput);
	}
}

TEST(regex_test, syntax_nourl_failurecase)
{
    // Arrange
    int testcasec = 1;
    std::string *testcases = new std::string[testcasec]
	{
		""
	};

    // Act
    for (int i = 0; i < testcasec; ++i)
    {
        auto input = testcases[i];

        std::tuple<std::string, std::string, std::string> output;

        // Assert

        EXPECT_FALSE(regex::validateSyntax(input, output));
    }
}
#pragma endregion

#pragma region Flag Argument Pair Regex Tests

TEST(regex_test, parse_flargs_successcase)
{
	// Arrange
    std::map<std::string,       // Input
             std::map<          // Map of flag-argument pairs:
                std::string,	//	key   = flag
				std::string>	//	value = argument
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

		// Act & Assert
		
		ASSERT_NO_FATAL_FAILURE(regex::parseFlargPairs(input, output));

		ASSERT_EQ(output.size(), expected_output.size());

		for (auto const& pair : output)
        {
			auto key = pair.first;

            EXPECT_EQ(output[key], expected_output[key]);  
		}
	}
}

TEST(regex_test, parse_flargs_failurecase)
{
    int testcasec = 5;
    std::string *testcases = new std::string[testcasec]
	{
		"-singleHyphenMultiChar", 
		"--d oubleHyphenSingleChar", 
		"--flag with twoArguments", 
		"noHyphenFlag", 
		"---moreThanTwoHyphens"
    };

	std::map<std::string, std::string> output;

	for (int i = 0; i < testcasec; ++i)
    {
        auto input = testcases[i];

        ASSERT_EXIT(regex::parseFlargPairs(input, output), ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
	}
}

#pragma endregion

#pragma region URL Validation Tests

TEST(regex_test, url_validation_successcase)
{
	// Arrange has been taken care of.

	// Act & Assert 

	for (int i = 0; i < validURLc; ++i)
    {
        auto input = validURLs[i];

        EXPECT_TRUE(regex::validateURL(input));    
	}
}

TEST(regex_test, url_validation_failurecase)
{
	// Arrange
    int invalidURLc = 5;
    std::string *invalidURLs = new std::string[invalidURLc]
	{
		"notAURL", 
		"completely wrong string", 
		"bad.formatted.url/this-should-not-work", 
		"ptth//:this_is_not_a.valid/url", 
		"wwww.smallmistake.com/close_but_no_cigar"
    };

	// Act & Assert 

	for (int i = 0; i < invalidURLc; ++i)
    {
        auto input = invalidURLs[i];

        EXPECT_FALSE(regex::validateURL(input));
	}
}

#pragma endregion
