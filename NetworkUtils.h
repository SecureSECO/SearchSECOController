#pragma once
#include <vector>
#include <string>
struct HashData;

class NetworkUtils
{
public:
	/// <summary>
	/// Converts a list of Hashes into a big string that can be send to the database.
	/// The individual parts components of each entry will be seperated by a null character.
	/// The entries will be seperated by a new line.
	/// The first entry will be the header that you give this function.
	/// </summary>
	/// <param name="data">The hashes you want to put in the string</param>
	/// <param name="size">This is basically a second return value. The size of the string that is returned will be put in here. The initial value that you give it will be ignored</param>
	/// <param name="header">The header that you want infront of the string. Even if the header is empty, it will still put an end line in the beginning.</param>
	/// <returns>A char pointer to string with all the data in it</returns>
	static char* getAllDataFromHashes(std::vector<HashData> data, int& size, std::string header);

	static char* getHashDataFromHashes(std::vector<HashData> data, int& size);

	static std::string generateHeader(std::vector<std::string> components);
private:
	/// <summary>
	/// Adds a string to a char* buffer
	/// </summary>
	static void addStringToBuffer(char* buffer, int& pos, std::string adding);

};

