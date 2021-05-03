/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"

#include "Flags.h"
#include "Commands.h"
#include "Utils.h"

#include <map>
#include <filesystem>
#include <string>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

// Constants.
#define TEMPPATH "temp"
#define LINUX0DOT01_FILECOUNT 162
#define PYTHON_FILECOUNT 0
#define CPP_FILECOUNT 76
#define JS_FILECOUNT 0

// Dummy variables.
Flags flags;


int cloneAndCheck(std::map<std::string, bool> &dict, std::string url)
{
    Commands::downloadRepository(url, flags, TEMPPATH);
    int count = 0;
    for (const auto &dirEntry : recursive_directory_iterator(TEMPPATH))
    {
        if (utils::split(dirEntry.path().string(), '\\')[1] == ".git") 
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

TEST(IntegrationTestSpider, Linux0dot01)
{
    std::map<std::string, bool> files = 
    {
        {"temp\\fs\\bitmap.c", 0},
        {"temp\\fs\\bitmap.c.meta", 0},
        {"temp\\include\\linux\\head.h", 0},
        {"temp\\include\\linux\\head.h.meta", 0},
        {"temp\\kernel\\exit.c", 0},        
        {"temp\\kernel\\exit.c.meta", 0},
        {"temp\\lib\\close.c", 0},          
        {"temp\\lib\\close.c.meta", 0}
    };

    int count = cloneAndCheck(files, "https://github.com/zavg/linux-0.01");
   

    for (auto const &[key, val] : files)
    {
        EXPECT_TRUE(val);
    }

    EXPECT_EQ(count, (int)LINUX0DOT01_FILECOUNT);
}

TEST(IntegrationTestSpider, Cpp)
{
    std::map<std::string, bool> files = 
    {
        {"temp\\src\\endgame.cpp", 0},
        {"temp\\src\\endgame.cpp.meta", 0},
        {"temp\\src\\pawns.h", 0},
        {"temp\\src\\pawns.h.meta", 0},
        {"temp\\src\\syzygy\\tbprobe.h", 0}, 
        {"temp\\src\\syzygy\\tbprobe.h.meta", 0},
        {"temp\\src\\syzygy\\tbprobe.cpp", 0},
        {"temp\\src\\syzygy\\tbprobe.cpp.meta", 0},
    };

    int count = cloneAndCheck(files, "https://github.com/mcostalba/Stockfish");
   

    for (auto const &[key, val] : files)
    {
        EXPECT_TRUE(val);
    }

    EXPECT_EQ(count, (int)CPP_FILECOUNT);
}

TEST(IntegrationTestSpider, Python)
{
    std::map<std::string, bool> files = 
    {
    };

    int count = cloneAndCheck(files, "https://github.com/python/typing");
   

    for (auto const &[key, val] : files)
    {
        EXPECT_TRUE(val);
    }

    EXPECT_EQ(count, (int)PYTHON_FILECOUNT);
}

TEST(IntegrationTestSpider, JavaScript)
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

