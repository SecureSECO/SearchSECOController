/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

/*
NOTE: These tests depend on the used GitHub repositories. If a test fails, make sure the repository is not changed in an unexpected way.
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "moduleFacades.h"
#include "flags.h"
#include "utils.h"

// External includes
#include <filesystem>
#include <map>
#include <string>
#include "print.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

// Constants.
#define TEMPPATH "temp"
#define LINUX0DOT01_FILECOUNT 162
#define CPP_FILECOUNT 76
#define PHP_FILECOUNT 0
#define JS_FILECOUNT 4


int cloneAndCheck(std::map<std::string, bool> &dict, std::string url)
{
	// Dummy variables.
	Flags spiderFlags;

	moduleFacades::downloadRepository(url, spiderFlags, TEMPPATH);
	int count = 0;
	for (const auto &dirEntry : recursive_directory_iterator(TEMPPATH))
	{
		if (Utils::split(dirEntry.path().string(), '\\')[1] == ".git") 
		{
			continue;
		}

		count++;
		if(dict.count(dirEntry.path().string()) > 0 && !dict[dirEntry.path().string()]) 
		{
			dict[dirEntry.path().string()] = true;
		}
	}
	return count;
}

TEST(integrationSpider, linux0dot01)
{
	std::map<std::string, bool> files = 
	{
		{TEMPPATH + std::string("\\fs\\bitmap.c"), 0},    
		{TEMPPATH + std::string("\\fs\\bitmap.c.meta"), 0},
		{TEMPPATH + std::string("\\include\\linux\\head.h"), 0},
		{TEMPPATH + std::string("\\include\\linux\\head.h.meta"), 0},
		{TEMPPATH + std::string("\\kernel\\exit.c"), 0},        
		{TEMPPATH + std::string("\\kernel\\exit.c.meta"), 0},
		{TEMPPATH + std::string("\\lib\\close.c"), 0},          
		{TEMPPATH + std::string("\\lib\\close.c.meta"), 0}
	};

	int count = cloneAndCheck(files, "https://github.com/zavg/linux-0.01");
   

	for (auto const &[key, val] : files)
	{
		EXPECT_TRUE(val);
	}

	EXPECT_EQ(count, (int)LINUX0DOT01_FILECOUNT);
}

TEST(integrationSpider, cpp)
{
	std::map<std::string, bool> files = 
	{
		{TEMPPATH + std::string("\\src\\endgame.cpp"), 0},
		{TEMPPATH + std::string("\\src\\endgame.cpp.meta"), 0},
		{TEMPPATH + std::string("\\src\\pawns.h"), 0},
		{TEMPPATH + std::string("\\src\\pawns.h.meta"), 0},
		{TEMPPATH + std::string("\\src\\syzygy\\tbprobe.h"), 0}, 
		{TEMPPATH + std::string("\\src\\syzygy\\tbprobe.h.meta"), 0},
		{TEMPPATH + std::string("\\src\\syzygy\\tbprobe.cpp"), 0},
		{TEMPPATH + std::string("\\src\\syzygy\\tbprobe.cpp.meta"), 0},
	};

	int count = cloneAndCheck(files, "https://github.com/mcostalba/Stockfish");
   

	for (auto const &[key, val] : files)
	{
		EXPECT_TRUE(val);
	}

	EXPECT_EQ(count, (int)CPP_FILECOUNT);
}

TEST(integrationSpider, php)
{
	std::map<std::string, bool> files = 
	{
	};

	int count = cloneAndCheck(files, "https://github.com/PHP-DI/demo");
   
	
	for (auto const &[key, val] : files)
	{
		EXPECT_TRUE(val);
	}

	EXPECT_EQ(count, (int)PHP_FILECOUNT);
}

TEST(integrationSpider, JavaScript)
{
	std::map<std::string, bool> files = 
	{
	};

	int count = cloneAndCheck(files, "https://github.com/git-tips/tips");
   

	for (auto const &[key, val] : files)
	{
		EXPECT_TRUE(val);
	}

	EXPECT_EQ(count, (int)JS_FILECOUNT);
}

TEST(integrationSpider, wrongURLFailurecase)
{
	// Dummy variables.
	Flags spiderFlags;
	std::string url = "https://secureseco.org";

	auto[ result, commithash, unchangedFiles ] = moduleFacades::downloadRepository(url, spiderFlags, TEMPPATH);

	EXPECT_EQ(result.size(), 0);
	EXPECT_EQ(commithash, "");
	EXPECT_EQ(unchangedFiles.size(), 0);
}
