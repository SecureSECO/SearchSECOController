/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#include "pch.h"
#include "../parser/Parser/XmlParser.h"
#include "../NetworkUtils.h"
#include "../NetworkUtils.cpp"
#include "../parser/Parser/HashData.h"

TEST(Networking_get__all_data_from_hashes_tests, basic_test)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7) };
	int size;
	char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER");
	char* s = "HEADER\nHASH?FUNCTION?FILENAME?5?0\n";
	for (int i = 0; i < size; i++)
	{
		EXPECT_EQ(buffer[i], s[i]);
	}
	EXPECT_EQ(size, 34);
}

TEST(Networking_get__all_data_from_hashes_tests, empty_test)
{
	std::vector<HashData> hashes = { };
	int size;
	char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER");
	char* s = "HEADER\n";
	for (int i = 0; i < size; i++)
	{
		EXPECT_EQ(buffer[i], s[i]);
	}
	EXPECT_EQ(size, 7);
}

TEST(Networking_get__all_data_from_hashes_tests, bigger_test)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7), HashData("HASH", "FUNCTION", "FILENAME", 2, 7), HashData("HASH", "FUNCTION", "FILENAME", 1, 7) };
	int size;
	char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER");
	char* s = "HEADER\nHASH?FUNCTION?FILENAME?5?0\nHASH?FUNCTION?FILENAME?2?0\nHASH?FUNCTION?FILENAME?1?0\n";
	for (int i = 0; i < size; i++)
	{
		EXPECT_EQ(buffer[i], s[i]);
	}
	EXPECT_EQ(size, 88);
}

TEST(Networking_get_hash_data_from_hashes_tests, basic_test)
{
	std::vector<HashData> hashes = { HashData("HASH0", "", "", 0, 7) };
	int size;
	char* buffer = NetworkUtils::getHashDataFromHashes(hashes, size);
	char* s = "HASH0\n";
	for (int i = 0; i < size; i++)
	{
		EXPECT_EQ(buffer[i], s[i]);
	}
	EXPECT_EQ(size, 6);
}

TEST(Networking_get_hash_data_from_hashes_tests, empty_test)
{
	std::vector<HashData> hashes = { };
	int size;
	char* buffer = NetworkUtils::getHashDataFromHashes(hashes, size);
	char* s = "";
	for (int i = 0; i < size; i++)
	{
		EXPECT_EQ(buffer[i], s[i]);
	}
	EXPECT_EQ(size, 0);
}

TEST(Networking_get_hash_data_from_hashes_tests, bigger_test)
{
	std::vector<HashData> hashes = { HashData("HASH0", "", "", 0, 7), HashData("HASH1", "", "", 0, 7), HashData("HASH2", "", "", 0, 7), HashData("HASH3", "", "", 0, 7) };
	int size;
	char* buffer = NetworkUtils::getHashDataFromHashes(hashes, size);
	char* s = "HASH0\nHASH1\nHASH2\nHASH3\n";
	for (int i = 0; i < size; i++)
	{
		EXPECT_EQ(buffer[i], s[i]);
	}
	EXPECT_EQ(24, size);
}
