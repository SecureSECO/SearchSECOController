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
		{"nonexistantButAcceptedCommand -V 3 --flag argument", std::make_tuple("nonexistantButAcceptedCommand", "", "-V 3 --flag argument")},
		{"argumentOutOfRangeButAccepted -V 15", std::make_tuple("argumentOutOfRangeButAccepted", "", "-V 15")},
		{"unnessecaryWhiteSpacesIgnored    -h      ", std::make_tuple("unnessecaryWhiteSpacesIgnored", "", "-h")}
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

}

TEST(regex_test, syntax_nourl_failurecase)
{

}

TEST(regex_test, syntax_url_failurecase)
{
	
}
#pragma endregion
