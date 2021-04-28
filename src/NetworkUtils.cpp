/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#include "NetworkUtils.h"
#include "Print.h"

void NetworkUtils::addStringToBuffer(char* buffer, int& pos, std::string adding)
{
	for (char c : adding)
	{
		buffer[pos++] = c;
	}
}

std::map<std::string, std::vector<HashData*>>* NetworkUtils::transformHashList(std::vector<HashData>& hashes)
{
	std::map<std::string, std::vector<HashData*>>* list = new std::map<std::string, std::vector<HashData*>>();
	for (int i = 0; i < hashes.size(); i++)
	{

		list->at(hashes[i].fileName).push_back(&(hashes[i]));
		if (list->at(hashes[i].fileName).size() > 1 &&
			list->at(hashes[i].fileName)[list->at(hashes[i].fileName).size() - 1]->lineNumber <
			list->at(hashes[i].fileName)[list->at(hashes[i].fileName).size() - 2]->lineNumber)
		{
			// Do sort or something.
		}
	}
	return list;
}

int NetworkUtils::getAuthors(std::map<HashData, std::vector<std::string>>& authors, std::map<std::string, std::vector<HashData*>>* hashes, AuthorData& rawData)
{
	int authorSize = 0;
	for (auto const& key : rawData)
	{
		int currentEnd = 0;
		int hashesIndex = 0;
		int authorIndex = 0;
		while (hashes->size() < hashesIndex && rawData.size() < authorIndex)
		{

		}
	}
}

char* NetworkUtils::getAllDataFromHashes(std::vector<HashData> data, int& size, std::string header, AuthorData& authors)
{
	auto transformedData = transformHashList(data);
	// Calcutating the eventual size of the string before hand, 
	// so that we don't have to increase the size of the buffer.
	size = header.size() + 1;
	for (HashData hd : data)
	{
		size += hd.fileName.length() + hd.functionName.length() + hd.hash.length() + std::to_string(hd.lineNumber).length() + 1/*TODO: +1 vervangen voor authors*/;
		size += 5;
	}

	// Filling the buffer by first adding the header, and then each entry.
	char* buffer = new char[size];
	int pos = 0;
	addStringToBuffer(buffer, pos, header);
	buffer[pos++] = '\n';

	for (HashData hd : data)
	{
		// Hash|functionName|fileLocation|lineNumber|number_of_authors|author1_name|author1_mail|...
		addStringToBuffer(buffer, pos, hd.hash);
		buffer[pos++] = '?';
		addStringToBuffer(buffer, pos, hd.functionName);
		buffer[pos++] = '?';
		addStringToBuffer(buffer, pos, hd.fileName);
		buffer[pos++] = '?';
		addStringToBuffer(buffer, pos, std::to_string(hd.lineNumber));
		buffer[pos++] = '?';
		addStringToBuffer(buffer, pos, "0");
		buffer[pos++] = '\n';
	}
	return buffer;
}

char* NetworkUtils::getHashDataFromHashes(std::vector<HashData> data, int& size)
{
	// Calcutating the eventual size of the string before hand, 
	// so that we don't have to increase the size of the buffer.
	size = 0;
	for (HashData hd : data)
	{
		size += hd.hash.length();
		size += 1;
	}

	// Filling the buffer by first adding the header, and then each entry.
	char* buffer = new char[size];
	int pos = 0;

	for (HashData hd : data)
	{
		// hash
		addStringToBuffer(buffer, pos, hd.hash);
		buffer[pos++] = '\n';
	}

	return buffer;
}

std::string NetworkUtils::generateHeader(std::vector<std::string> components)
{
	if (components.size() == 0)
	{
		return "";
	}
	std::string output = components[0];
	for (int i = 1; i < components.size(); i++)
	{
		output.append('?' + components[i]);
	}
	return output;
}
