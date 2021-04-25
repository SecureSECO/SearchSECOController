#include "pch.h"

#include "regex.h"

#include <map>

#pragma region Syntax Regex Tests

int validURLc = 4;
std::string* validURLs = new std::string[validURLc]
{
	"http://github.com/UserName/project",
	"https://www.github.com/AnotherUser/project-with-hyphens", 
	"www.github.com/username-with-hyphens/CoolProject", 
	"github.com/short-url/but-long-hyphenated-project-containing-many-potential-files_which-we"
		"-would-like-to-spider-and-parse"
};

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

TEST(regex_test, syntax_url_successcase)
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
    int testcasec = 4;
    std::string *testcases =
        new std::string[testcasec]{"command argument notAFlag", "", "command -singleHyphenMultiCharFlag argument",
                                   "command --d oubleHyphenSingleCharFlag"};

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
