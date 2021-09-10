/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

/*
NOTE: These tests depend on the Linux0dot01 integration test of the spider.
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "moduleFacades.h"
#include "Spider.h"


//Constants.
#define TEMPPATH "temp"

//Dummy variables.
Flags parserFlags;

TEST(integrationParser, basic)
{
	std::vector<HashData> expected = 
	{
		{HashData("", "scrup", "kernel\\console.c", 67, 111)},
		{HashData("", "rw_hd", "kernel\\hd.c", 101, 117)},
	};

	std::string url = "https://github.com/zavg/linux-0.01";

	// Initialize spider.
	Spider *s = moduleFacades::setupSpider(url, parserFlags);

	moduleFacades::downloadRepo(s, url, parserFlags, TEMPPATH);
	auto hashes = moduleFacades::parseRepository(TEMPPATH, parserFlags);

	int count = 0;
	for(HashData hd : hashes) 
	{
		for(HashData expectedHd : expected) 
		{
			if (expectedHd.fileName == hd.fileName && expectedHd.functionName == hd.functionName)
			{
				EXPECT_EQ(expectedHd.fileName, hd.fileName);
				EXPECT_EQ(expectedHd.functionName, hd.functionName);
				EXPECT_EQ(expectedHd.lineNumber, hd.lineNumber);
				EXPECT_EQ(expectedHd.lineNumberEnd, hd.lineNumberEnd);
				count++;
			}
		}
	}
	EXPECT_EQ(count, expected.size());
}

TEST(integrationParser, failurecase)
{
	std::vector<HashData> expected = {
	};

	std::string url = "https://github.com/zavg/linux-0.01";

	// Initialize spider.
	Spider *s = moduleFacades::setupSpider(url, parserFlags);

	moduleFacades::downloadRepo(s, url, parserFlags, TEMPPATH);
	auto hashes = moduleFacades::parseRepository("wrongpath", parserFlags);

	int count = 0;
	for (HashData hd : hashes)
	{
		for (HashData expectedHd : expected)
		{
			if (expectedHd.fileName == hd.fileName && expectedHd.functionName == hd.functionName)
			{
				count++;
			}
		}
	}
	
	EXPECT_EQ(hashes.size(), expected.size());
	EXPECT_EQ(count, expected.size());
}
