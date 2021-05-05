/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "loguru/loguru.hpp"

#include "Print.h"
#include "Utils.h"
#include "Parser.h"
#include "NetworkUtils.h"

#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <functional>

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
	loguru::log(loguru::Verbosity_1, file, line, msg.c_str());
}

void print::log(std::string msg, const char* file, int line)
{
	loguru::log(loguru::Verbosity_INFO, file, line, msg.c_str());
}

void print::warn(std::string msg, const char* file, int line)
{
	loguru::log(loguru::Verbosity_WARNING, file, line, msg.c_str());
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
	if (n == 1) return singular;
	return singular + 's';
}

void print::version_full()
{
	std::string main_name = "searchseco";

	// Get subsystem versions.
	int systemc = 2;
	std::string* subsystems = new std::string[systemc]
	{
		"parser",
		"spider",
		//"database_api"
	};

	std::ifstream version_file;
	std::string version;

	// print version of the main program.
	version_file.open("VERSION");
	std::getline(version_file, version);

	print::printline(main_name + " version " + version);
	
	version_file.close();

	// Loop over the subsystems.
	for (int i = 0; i < systemc; ++i)
	{
		std::string system = subsystems[i];
		version_file.open(system + "/VERSION");

		std::getline(version_file, version);

		print::printline(">> " + system + " version " + version);
		
		version_file.close();
	}
}

void print::printHashMatches(std::vector<HashData> hashes, std::string databaseOutput, AuthorData authordata)
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

			// Method1_hash|method1_projectid|method1_version|method1_name|method1_fileLocation
			// |method1_lineNumber|number_of_authors|method1_authorid1|method1_authorid2|...
			std::vector<std::string> dbEntry = receivedHashes[hashes[i].hash];
			printline("\n" + hashes[i].functionName + " in file " + hashes[i].fileName + " line "
				+ std::to_string(hashes[i].lineNumber) + " was found in our database: ");
			printline("Function " + dbEntry[3] + " in project " + dbEntry[1]
				+ " in file " + dbEntry[4] + " line " + dbEntry[5]);
			printline("Authors of local function: ");
			for (std::string s : authors[hashes[i]])
			{
				utils::replace(s, '?', '\t');
				printline(s);
				authorsCopied[s]++;
			}
			printline("Authors of function found in database: ");

			matches++;
			for (int i = 7; i < 7 + std::stoi(dbEntry[6]); i++)
			{
				authorCopiedForm[dbEntry[i]]++;
				printline("\t" + dbEntry[i]);
			}
		}
	}
	
	printline("\nSummary:");
	printline("\tMatches: " + std::to_string(matches));
	printline("Local authors present in matches: ");
	for (auto const& x : authorsCopied)
	{
		printline(x.first + ": " + std::to_string(x.second));
	}
	printline("Authors present in database matches: ");
	for (auto const& x : authorCopiedForm)
	{
		printline("\t" + x.first + ": " + std::to_string(x.second));
	}
}
#pragma endregion Print
