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

		EXPECT_TRUE(regex::validateSyntax(input, output));

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
        
		EXPECT_TRUE(regex::validateSyntax(input, output));

		EXPECT_EQ(output, expectedOutput);
	}
}

TEST(regex_test, syntax_nourl_failurecase)
{
    // Arrange
    int testcasec = 2;
    std::string *testcases = new std::string[testcasec]
	{
		"command argument notAFlag", 
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
    std::map<std::string,                                // Input
             std::tuple<                                 // Output
                 std::tuple<std::string, std::string> *, // Array of flag-argument tuples: item1 = flag, item2 =
                                                         //		argument
                 int>>                                   // Length of the flag-argument tuple array
        testcases = 
	{
		{
			"-V 3 --output console", 
			std::make_tuple(
				new std::tuple<std::string, std::string>[2] 
				{
                    std::make_tuple("V", "3"), 
					std::make_tuple("output", "console")
				},
				2
			)
		},
        {
			"--cpu 8 --looksLikeAFlagButDoesNotExist itShouldStillBeParsedThough", 
			std::make_tuple(
				new std::tuple<std::string, std::string>[2] 
				{
                    std::make_tuple("cpu", "8"),
                    std::make_tuple("looksLikeAFlagButDoesNotExist", "itShouldStillBeParsedThough"), 
				},
				2
			)
		},
        {
			"-v", 
			std::make_tuple(
				new std::tuple<std::string, std::string>[1] 
				{
					std::make_tuple("v", "")
				},
				1
			)
		}
	};

	for (auto const& testcase : testcases)
    {
        auto input = testcase.first;
        
		auto expected_output_full = testcase.second;
        auto expected_output  = std::get<0>(expected_output_full);
        auto expected_outputc = std::get<1>(expected_output_full);

		std::tuple<std::string, std::string> *output;
        int outputc;

		// Act & Assert
		
		EXPECT_TRUE(regex::parseFlargPairs(input, output, outputc));

		EXPECT_EQ(outputc, expected_outputc);

		for (int i = 0; i < expected_outputc; ++i)
        {
            EXPECT_EQ(output[i], expected_output[i]);  
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

	std::tuple<std::string, std::string> *output;
    int outputc;

	for (int i = 0; i < testcasec; ++i)
    {
        auto input = testcases[i];

        EXPECT_FALSE(regex::parseFlargPairs(input, output, outputc));
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

}

#pragma endregion
