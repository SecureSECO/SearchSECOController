#include "NetworkUtils.h"
#include "parser/Parser/Parser.h"

void NetworkUtils::addStringToBuffer(char* buffer, int& pos, std::string adding)
{
	for (char c : adding)
	{
		buffer[pos++] = c;
	}
}

char* NetworkUtils::getDataFromHashes(std::vector<HashData> data, int& size, std::string header)
{
	// Calcutating the eventual size of the string before hand, 
	// so that we don't have to increase the size of the buffer.
	size = header.size() + 1;
	for (HashData hd : data)
	{
		size += hd.fileName.length() + hd.functionName.length() + hd.hash.length() + std::to_string(hd.lineNumber).length();
		size += 5;
	}

	// Filling the buffer by first adding the header, and then each entry.
	char* buffer = new char[size];
	int pos = 0;
	addStringToBuffer(buffer, pos, header);
	buffer[pos++] = '\n';

	for (HashData hd : data)
	{
		addStringToBuffer(buffer, pos, hd.fileName);
		buffer[pos++] = 0;
		addStringToBuffer(buffer, pos, std::to_string(hd.lineNumber));
		buffer[pos++] = 0;
		addStringToBuffer(buffer, pos, hd.functionName);
		buffer[pos++] = 0;
		addStringToBuffer(buffer, pos, hd.hash);
		buffer[pos++] = '\n';
	}

	return buffer;
}
