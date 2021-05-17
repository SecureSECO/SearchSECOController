/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "networkUtils.h"
#include "print.h"


inline
bool operator==(HashData const& lhs, HashData const& rhs)
{
	return lhs.hash == rhs.hash &&
		lhs.fileName == rhs.fileName &&
		lhs.functionName == rhs.functionName
		&& lhs.lineNumber == rhs.lineNumber
		&& lhs.lineNumberEnd == rhs.lineNumberEnd;
}

inline
bool operator<(HashData const& lhs, HashData const& rhs)
{
	if (lhs.hash != rhs.hash)
	{
		return lhs.hash < rhs.hash;
	}
	if (lhs.fileName != rhs.fileName)
	{
		return lhs.fileName < rhs.fileName;
	}
	if (lhs.functionName != rhs.functionName)
	{
		return lhs.functionName < rhs.functionName;
	}
	if (lhs.lineNumber != rhs.lineNumber)
	{
		return lhs.lineNumber < rhs.lineNumber;
	}
	return lhs.lineNumberEnd < rhs.lineNumberEnd;
}


void NetworkUtils::addStringToBuffer(char* buffer, int& pos, std::string adding)
{
	for (char c : adding)
	{
		buffer[pos++] = c;
	}
}

void NetworkUtils::addStringsToBuffer(char* buffer, int& pos, std::vector<std::string> adding)
{
	for (std::string s : adding)
	{
		addStringToBuffer(buffer, pos, s);
	}
}

void NetworkUtils::addHashDataToBuffer(char* buffer, int& pos, HashData& hd, std::map<HashData, std::vector<std::string>>& authors)
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
	addStringToBuffer(buffer, pos, std::to_string(authors[hd].size()));
	addStringsToBuffer(buffer, pos, authors[hd]);
	buffer[pos++] = '\n';
}

void NetworkUtils::transformHashList(std::vector<HashData>& hashes, std::map<std::string, std::vector<HashData*>> &output)
{
	for (int i = 0; i < hashes.size(); i++)
	{
		output[hashes[i].fileName].push_back(&(hashes[i]));
		// Sort if they are out of order.
		if (output[hashes[i].fileName].size() > 1 &&
			output[hashes[i].fileName][((int)output[hashes[i].fileName].size()) - 1]->lineNumber <
			output[hashes[i].fileName][((int)output[hashes[i].fileName].size()) - 2]->lineNumber)
		{
			int j = ((int)output[hashes[i].fileName].size()) - 1;
			while (j > 0 &&
				output[hashes[i].fileName][j]->lineNumber <
				output[hashes[i].fileName][j - 1]->lineNumber)
			{
				HashData* temp = output[hashes[i].fileName][j];
				output[hashes[i].fileName][j] = output[hashes[i].fileName][j - 1];
				output[hashes[i].fileName][j - 1] = temp;
				j--;
			}
		}
	}
}

int NetworkUtils::getAuthors(std::map<HashData, std::vector<std::string>>& authors, std::map<std::string, std::vector<HashData*>>& hashes, AuthorData& rawData)
{
	int authorSize = 0;
	for (auto const& key : rawData)
	{
		int currentEnd = -1;
		int hashesIndex = -1;
		int authorIndex = 0;
		std::map<std::string, int> dupes;
		
		while (hashesIndex < (int)hashes[key.first].size() || authorIndex < (int)rawData[key.first].size())
		{
			if (authorIndex == (int)rawData[key.first].size() || currentEnd < rawData[key.first][authorIndex].line)
			{
				hashesIndex++;
				if (hashesIndex >= (int)hashes[key.first].size())
				{
					break;
				}
				if (authorIndex > 0)
				{
					authorIndex--;
				}
				currentEnd = hashes[key.first][hashesIndex]->lineNumberEnd;
				dupes = std::map<std::string, int>();
			}
			if (hashes[key.first][hashesIndex]->lineNumber <=
				rawData[key.first][authorIndex].line + rawData[key.first][authorIndex].numLines)
			{
				CodeBlock cd = rawData[key.first][authorIndex];
				std::string toAdd = "?" + cd.commit->author + "?" + cd.commit->authorMail;
				if (dupes[toAdd] == 0)
				{
					authorSize += toAdd.length();
					authors[*hashes[key.first][hashesIndex]].push_back(toAdd);
					dupes[toAdd] = 1;
				}
			}
			authorIndex++;
		}
	}
	return authorSize;
}

const char* NetworkUtils::getAllDataFromHashes(std::vector<HashData> data, int& size, std::string header, AuthorData& authors)
{
	// For getting the corresponding authors for each method,
	// we first need to transform the list of hashes a bit.
	std::map<std::string, std::vector<HashData*>> transformedHashes;
	transformHashList(data, transformedHashes);
	std::map<HashData, std::vector<std::string>> authorSendData;
	// Calcutating the eventual size of the string before hand, 
	// so that we don't have to increase the size of the buffer.
	size = header.size() + 1 + getAuthors(authorSendData, transformedHashes, authors);
	for (HashData hd : data)
	{
		size += hd.fileName.length() + 
			hd.functionName.length() + 
			hd.hash.length() + 
			std::to_string(hd.lineNumber).length() + 
			std::to_string(authorSendData[hd].size()).length();
		// Plus 5 for the seperators.
		size += 5;
	}

	// Filling the buffer by first adding the header, and then each entry.
	char* buffer = new char[size];
	int pos = 0;
	addStringToBuffer(buffer, pos, header);
	buffer[pos++] = '\n';

	for (HashData hd : data)
	{
		addHashDataToBuffer(buffer, pos, hd, authorSendData);
	}
	return buffer;
}

const char* NetworkUtils::getHashDataFromHashes(std::vector<HashData> data, int& size)
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
		if (components[i] == "")
		{
			// TODO: we might want to change this later, dependent on what the database wants.
			components[i] = "-";
		}
		output.append('?' + components[i]);
	}
	return output;
}
