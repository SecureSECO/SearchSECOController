#include "pch.h"

#include "regex.h"

#include <map>

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
        {"command http://github.com/UserName/project --flag argument", 
			std::make_tuple("command", "http://github.com/UserName/project", "--flag argument")},
        {"command https://www.github.com/AnotherUser/project-with-hyphens", 
			std::make_tuple("command", "https://www.github.com/AnotherUser/project-with-hyphens", "")},
        {"command www.github.com/username-with-hyphens/CoolProject --output console", 
			std::make_tuple("command", "www.github.com/username-with-hyphens/CoolProject", "--output console")},
        {"command github.com/short-url/but-long-hyphenated-project-containing-many-potential-files_which-we-would-"
			"like-to-spider-and-parse",
			std::make_tuple("command", "github.com/short-url/but-long-hyphenated-project-containing-many-potential"
				"-files_which-we-would-like-to-spider-and-parse", "")},
	};
}

TEST(regex_test, syntax_nourl_failurecase)
{

}

TEST(regex_test, syntax_url_failurecase)
{
	
}
#pragma endregion
