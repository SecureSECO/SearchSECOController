/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "networkUtils.h"


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

void NetworkUtils::addHashDataToBuffer(
	char* buffer,
	int& pos, 
	HashData& hd, 
	std::map<HashData, 
	std::vector<std::string>>& authors)
{
	// Hash|functionName|fileLocation|lineNumber|number_of_authors|author1_name|author1_mail|...
	addStringToBuffer(buffer, pos, hd.hash);
	buffer[pos++] = INNER_DELIMITER;
	addStringToBuffer(buffer, pos, hd.functionName);
	buffer[pos++] = INNER_DELIMITER;
	addStringToBuffer(buffer, pos, hd.fileName);
	buffer[pos++] = INNER_DELIMITER;
	addStringToBuffer(buffer, pos, std::to_string(hd.lineNumber));
	buffer[pos++] = INNER_DELIMITER;
	addStringToBuffer(buffer, pos, std::to_string(authors[hd].size()));
	addStringsToBuffer(buffer, pos, authors[hd]);
	buffer[pos++] = ENTRY_DELIMITER;
}

void NetworkUtils::transformHashList(
	std::vector<HashData>& hashes, 
	std::map<std::string, 
	std::vector<HashData*>> &output)
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

int NetworkUtils::getAuthors(
	std::map<HashData, std::vector<std::string>>& authors, 
	std::map<std::string, std::vector<HashData*>>& hashes, 
	AuthorData& rawData)
{
	int authorSize = 0;
	for (auto const& key : rawData)
	{
		int currentEnd = -1, hashesIndex = -1, authorIndex = 0;
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
				std::string toAdd = INNER_DELIMITER + cd.commit->author + INNER_DELIMITER + cd.commit->authorMail;
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

const char* NetworkUtils::getAuthorStringToSend(const std::map<std::string, int>& authors, int& size)
{
	// First, calculate the size, so we don't have to expand it later.
	size = 0;
	for (const auto& x : authors)
	{
		size += x.first.length() + 1;
	}
	char* data = new char[size];
	// Create the string.
	int pos = 0;
	for (const auto& x : authors)
	{
		addStringToBuffer(data, pos, x.first);
		data[pos++] = ENTRY_DELIMITER;
	}
	return data;
}

const char* NetworkUtils::getProjectsRequest(
	const std::map<std::pair<std::string, std::string>,
	int>& projects, 
	int& size)
{
	// First, calculate the size, so we don't have to expand it later.
	size = 0;
	for (const auto& x : projects)
	{
		size += x.first.first.length() + x.first.second.length() + 2;
	}
	char* data = new char[size];
	// Create the string.
	int pos = 0;
	for (auto x : projects)
	{
		addStringToBuffer(data, pos, x.first.first);
		data[pos++] = INNER_DELIMITER;
		addStringToBuffer(data, pos, x.first.second);
		data[pos++] = ENTRY_DELIMITER;
	}
	return data;
}

const char* NetworkUtils::getProjectRequest(const std::pair<std::string, std::string>& project, int& size)
{
	// First, calculate the size, so we don't have to expand it later.
	size = project.first.length() + project.second.length() + 2;
	char* data = new char[size];
	// Create the string.
	int pos = 0;
	addStringToBuffer(data, pos, project.first);
	data[pos++] = INNER_DELIMITER;
	addStringToBuffer(data, pos, project.second);
	data[pos++] = ENTRY_DELIMITER;
	return data;
}

const char* NetworkUtils::getJobsRequest(const std::vector<std::string>& urls, int& size)
{
	// First, calculate the size, so we don't have to expand it later.
	size = 0;
	for (const auto& x : urls)
	{
		size += x.length() + 1 + 2;
	}
	char* data = new char[size];
	// Create the string.
	int pos = 0;
	for (auto x : urls)
	{
		addStringToBuffer(data, pos, x);
		data[pos++] = INNER_DELIMITER;
		addStringToBuffer(data, pos, "1");
		data[pos++] = ENTRY_DELIMITER;
	}
	return data;
}

const char *NetworkUtils::getUpdateJobRequest(std::string jobid, std::string jobTime, int &size)
{
	// First, calculate the size, so we don't have to expand it later.
	size = jobid.length() + jobTime.length() + 2;
	char *data = new char[size];
	// Create the string.
	int pos = 0;
	addStringToBuffer(data, pos, jobid);
	data[pos++] = INNER_DELIMITER;
	addStringToBuffer(data, pos, jobTime);
	data[pos++] = ENTRY_DELIMITER;
	return data;
}

const char *NetworkUtils::getFinishJobRequest(std::string jobid, std::string jobTime, int code, std::string reason,
											  int &size)
{
	// First, calculate the size, so we don't have to expand it later.
	size = jobid.length() + jobTime.length() + std::to_string(code).length() + reason.length() + 4;
	char *data = new char[size];
	// Create the string.
	int pos = 0;
	addStringToBuffer(data, pos, jobid);
	data[pos++] = INNER_DELIMITER;
	addStringToBuffer(data, pos, jobTime);
	data[pos++] = INNER_DELIMITER;
	addStringToBuffer(data, pos, std::to_string(code));
	data[pos++] = INNER_DELIMITER;
	addStringToBuffer(data, pos, reason);
	data[pos++] = ENTRY_DELIMITER;
	return data;
}

const char* NetworkUtils::getUploadCrawlRequest(const CrawlData& urls, std::string id, int& size)
{
	// First, calculate the size, so we don't have to expand it later.
	// Initial size is for the header.
	size = std::to_string(urls.finalProjectId).length() + id.length() + 2;
	for (const auto& x : urls.URLImportanceList)
	{
		size += std::get<0>(x).length() + std::to_string(std::get<1>(x)).length() +
				std::to_string(std::get<2>(x)).length() + 3;
	}
	for (auto const &language : urls.languages)
	{
		size += language.first.length() + std::to_string(language.second).length() + 2;
	}
	char* data = new char[size];
	// Create the string.
	int pos = 0;

	addStringToBuffer(data, pos, std::to_string(urls.finalProjectId));
	data[pos++] = INNER_DELIMITER;
	addStringToBuffer(data, pos, id);
	data[pos++] = ENTRY_DELIMITER;

	for (auto const &language : urls.languages)
	{
		addStringToBuffer(data,pos, language.first);
		data[pos++] = INNER_DELIMITER;
		addStringToBuffer(data, pos, std::to_string(language.second));
		data[pos++] = INNER_DELIMITER;
	}

	data[pos - 1] = ENTRY_DELIMITER;

	for (auto x : urls.URLImportanceList)
	{
		addStringToBuffer(data, pos, std::get<0>(x));
		data[pos++] = INNER_DELIMITER;
		addStringToBuffer(data, pos, std::to_string(std::get<1>(x)));
		data[pos++] = INNER_DELIMITER;
		addStringToBuffer(data, pos, std::to_string(std::get<2>(x)));
		data[pos++] = ENTRY_DELIMITER;
	}
	return data;
}

const char* NetworkUtils::getAllDataFromHashes(std::vector<HashData>& data, int& size,
	std::string header, AuthorData& authors, std::string prevCommitTime, std::vector<std::string> unchangedFiles)
{
	// For getting the corresponding authors for each method,
	// we first need to transform the list of hashes a bit.
	std::map<std::string, std::vector<HashData*>> transformedHashes;
	transformHashList(data, transformedHashes);
	std::map<HashData, std::vector<std::string>> authorSendData;
	// Calculating the eventual size of the string before hand, 
	// so that we don't have to increase the size of the buffer.
	size = header.size() + 1 + getAuthors(authorSendData, transformedHashes, authors) 
		+ 1 + 1 + prevCommitTime.size();
	
	for (std::string s : unchangedFiles)
	{
		size += 1 + s.length();
	}
	
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
	buffer[pos++] = ENTRY_DELIMITER;

	addStringToBuffer(buffer, pos, prevCommitTime);
	buffer[pos++] = ENTRY_DELIMITER;

	for (std::string s : unchangedFiles)
	{
		addStringToBuffer(buffer, pos, s);
		buffer[pos++] = INNER_DELIMITER;
	}
	buffer[pos++] = ENTRY_DELIMITER;

	for (HashData hd : data)
	{
		addHashDataToBuffer(buffer, pos, hd, authorSendData);
	}
	
	return buffer;
}

const char* NetworkUtils::getHashDataFromHashes(std::vector<HashData>& data, int& size)
{
	// Calculating the eventual size of the string before hand, 
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
		buffer[pos++] = ENTRY_DELIMITER;
	}

	return buffer;
}

std::string NetworkUtils::generateHeader(std::vector<std::string> &components)
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
			components[i] = "-";
		}
		output.append(INNER_DELIMITER + components[i]);
	}
	return output;
}
