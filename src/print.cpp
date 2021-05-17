/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "print.h"
#include "utils.h"

// Parser includes
#include "Parser.h"
#include "NetworkUtils.h"

// External includes
#include "loguru/loguru.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <stdlib.h>


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

#pragma region Logging

void print::debug(std::string msg, const char* file, int line)
{
	loguru::log(loguru::Verbosity_1, file, line, "%s", msg.c_str());
}

void print::log(std::string msg, const char* file, int line)
{
	loguru::log(loguru::Verbosity_INFO, file, line, "%s", msg.c_str());
}

void print::warn(std::string msg, const char* file, int line)
{
	loguru::log(loguru::Verbosity_WARNING, file, line, "%s", msg.c_str());
}

#pragma endregion


#pragma region Print

void print::printline(std::string str)
{
	std::cout << str << '\n';
}

std::string print::tab(int n)
{
	std::string result = "";
	for (int i = 0; i < n; ++i)
	{
		result += "\t";
	}
	return result;
}

std::string print::encapsulate(std::string str, char c)
{
	return c + str + c;
}

std::string print::quote(std::string str)
{
	return encapsulate(str, '\"');
}

std::string print::plural(std::string singular, int n)
{
	if (n == 1)
	{
		return singular;
	}
	return singular + 's';
}

void print::versionFull()
{
	std::string mainName = "searchseco";

	// Get subsystem versions.
	int systemc = 3;
	std::string* subsystems = new std::string[systemc]
	{
		"parser",
		"crawler",
		"spider",
		//"database_api"
	};

	std::ifstream versionFile;
	std::string version;

	// print version of the main program.
	versionFile.open("VERSION");
	std::getline(versionFile, version);

	print::printline(mainName + " version " + version);
	
	versionFile.close();

	// Loop over the subsystems.
	for (int i = 0; i < systemc; ++i)
	{
		std::string system = subsystems[i];
		versionFile.open(system + "/VERSION");

		std::getline(versionFile, version);

		print::printline(">> " + system + " version " + version);
		
		versionFile.close();
	}
}
#pragma endregion Print

#pragma region Matches

void printMatches::printHashMatches(std::vector<HashData> hashes, std::string databaseOutput, AuthorData authordata)
{
	std::map<std::string, std::vector<std::string>> receivedHashes = {};
	// Seperate the response we got into its individual entries.
	std::vector<std::string> dbentries = utils::split(databaseOutput, '\n');
	for (std::string entry : dbentries)
	{
		std::vector<std::string> entrySplitted = utils::split(entry, '?');

		receivedHashes[entrySplitted[0]] = entrySplitted;
			
	}

	// Author data
	std::map<std::string, std::vector<HashData*>> transformedList;
	std::map<HashData, std::vector<std::string>> authors;
	NetworkUtils::transformHashList(hashes, transformedList);
	NetworkUtils::getAuthors(authors, transformedList, authordata);


	int matches = 0;
	std::map<std::string, int> authorCopiedForm;
	std::map<std::string, int> authorsCopied;
	// Add all hashes we got localy to a map.
	for (int i = 0; i < hashes.size(); i++)
	{
		if (receivedHashes.count(hashes[i].hash) > 0)
		{
			matches++;
			printMatch(hashes[i], receivedHashes, authors, authorCopiedForm, authorsCopied);
		}
	}
	printSummary(authorCopiedForm, authorsCopied, matches);
}

void printMatches::printMatch(
	HashData hash, 
	std::map<std::string, std::vector<std::string>>& receivedHashes, 
	std::map<HashData, std::vector<std::string>>& authors, 
	std::map<std::string, int>& authorCopiedForm, 
	std::map<std::string, int>& authorsCopied
) 
{
	std::vector<std::string> dbEntry = receivedHashes[hash.hash];
	print::printline("\n" + hash.functionName + " in file " + hash.fileName + " line "
		+ std::to_string(hash.lineNumber) + " was found in our database: ");
	print::printline("Function " + dbEntry[3] + " in project " + dbEntry[1]
		+ " in file " + dbEntry[4] + " line " + dbEntry[5]);
	print::printline("Authors of local function: ");
	for (std::string s : authors[hash])
	{
		utils::replace(s, '?', '\t');
		print::printline(s);
		authorsCopied[s]++;
	}
	print::printline("Authors of function found in database: ");

	for (int i = 7; i < 7 + std::stoi(dbEntry[6]); i++)
	{
		authorCopiedForm[dbEntry[i]]++;
		print::printline("\t" + dbEntry[i]);
	}
}

void printMatches::printSummary(std::map<std::string, int> authorCopiedForm, std::map<std::string, int> authorsCopied, int matches) 
{
	
	print::printline("\nSummary:");
	print::printline("\tMatches: " + std::to_string(matches));
	print::printline("Local authors present in matches: ");
	for (auto const& x : authorsCopied)
	{
		print::printline(x.first + ": " + std::to_string(x.second));
	}
	print::printline("Authors present in database matches: ");
	for (auto const& x : authorCopiedForm)
	{
		print::printline("\t" + x.first + ": " + std::to_string(x.second));
	}
}

#pragma endregion Matches
