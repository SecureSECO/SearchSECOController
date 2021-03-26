#include "pch.h"
#include "../parser/Parser/XmlParser.h"
#include "../NetworkUtils.h"
#include "../NetworkUtils.cpp"

TEST(Networking_get_data_from_hashes_tests, basic_test)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5) };
	int size;
	char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER");
	char* s = "HEADER\nFILENAME\0005\000FUNCTION\000HASH\n";
	for (int i = 0; i < size; i++)
	{
		EXPECT_EQ(buffer[i], s[i]);
	}
}
