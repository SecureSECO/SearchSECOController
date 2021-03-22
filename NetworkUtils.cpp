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
	size = header.size() + 1;
	for (HashData hd : data)
	{
		size += hd.fileName.length() + hd.functionName.length() + hd.hash.length() + std::to_string(hd.lineNumber).length();
		size += 5;
	}
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
