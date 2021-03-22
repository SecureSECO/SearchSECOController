#pragma once
#include <vector>
#include <string>
struct HashData;

class NetworkUtils
{
public:
	static char* getDataFromHashes(std::vector<HashData> data, int& size, std::string header);
	static void addStringToBuffer(char* buffer, int& pos, std::string adding);

};

