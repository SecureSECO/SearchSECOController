/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "print.h"
#include "databaseRequests.h"
#include "utils.h"

// External includes
#include "loguru/loguru.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

inline bool operator<(HashData const &lhs, HashData const &rhs)
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

void print::debug(std::string msg, const char *file, int line)
{
	loguru::log(loguru::Verbosity_1, file, line, "%s", msg.c_str());
}

void print::log(std::string msg, const char *file, int line)
{
	loguru::log(loguru::Verbosity_INFO, file, line, "%s", msg.c_str());
}

void print::warn(std::string msg, const char *file, int line)
{
	loguru::log(loguru::Verbosity_WARNING, file, line, "%s", msg.c_str());
}

void print::loguruSetSilent()
{
	loguru::g_stderr_verbosity = loguru::Verbosity_OFF;
}

void print::loguruResetThreadName()
{
	loguru::set_thread_name("controller");
}

#pragma endregion

#pragma region Print
std::string line(std::string str)
{
	return str + '\n';
}

void print::printline(std::string str)
{
	std::cout << line(str);
}

void print::writelineToFile(std::string str, std::ofstream &file)
{
	file << line(str);
}

void print::printAndWriteToFile(std::string str, std::ofstream &file)
{
	print::printline(str);
	print::writelineToFile(str, file);
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
	std::string mainName = "SearchSECOController";

	// Get subsystem versions.
	int systemc = 3;
	std::string *subsystems = new std::string[systemc]{
		"SearchSECOCrawler",
		"SearchSECOSpider",
		"SearchSECOParser",
	};

	std::ifstream versionFile;
	std::string version;

	// Print version of the main program.
	versionFile.open("VERSION");
	std::getline(versionFile, version);

	print::printline(mainName + "\tversion " + version);

	versionFile.close();

	// Loop over the subsystems.
	for (int i = 0; i < systemc; ++i)
	{
		std::string system = subsystems[i];
		versionFile.open(system + "/VERSION");

		std::getline(versionFile, version);

		print::printline(">> " + system + "\tversion " + version);

		versionFile.close();
	}
}
#pragma endregion Print

#pragma region Matches

void PrintMatches::printHashMatches(std::vector<HashData> &hashes, std::string databaseOutput, AuthorData &authordata,
									EnvironmentDTO *env, std::string url)
{
	std::map<std::string, std::vector<Method>> receivedHashes = {};

	std::map<std::string, int> projectMatches;
	std::set<std::pair<std::string, std::string>> projectVersions;
	std::map<std::string, int> dbAuthors;
	std::map<std::string, std::vector<std::string>> dbProjects;
	std::map<std::string, std::vector<std::string>> authorIdToName;

	// Separate the response we got into its individual entries.
	std::vector<std::string> dbentries = Utils::split(databaseOutput, ENTRY_DELIMITER);
	parseDatabaseHashes(dbentries, receivedHashes, projectMatches, projectVersions, dbAuthors);

	getDatabaseAuthorAndProjectData(projectVersions, dbAuthors, dbProjects, authorIdToName, env);

	// Author data.
	std::map<std::string, std::vector<HashData *>> transformedList;
	std::map<HashData, std::vector<std::string>> authors;
	NetworkUtils::transformHashList(hashes, transformedList);
	NetworkUtils::getAuthors(authors, transformedList, authordata);

	auto report = PrintMatches::setupOutputReport(url);

	int matches = 0;
	std::map<std::string, int> authorCopiedForm;
	std::map<std::string, int> authorsCopied;
	std::vector<std::pair<HashData *, Method>> vulnerabilities;

	// Add all hashes we got locally to a map.
	std::map<std::string, std::vector<HashData>> hashMethods = {};
	for (int i = 0; i < hashes.size(); i++)
	{
		hashMethods[hashes[i].hash].push_back(hashes[i]);
	}
	for (const auto &match : receivedHashes)
	{
		matches++;
		printMatch(hashMethods[match.first], match.second, authors, authorCopiedForm, authorsCopied, vulnerabilities,
				   dbProjects, authorIdToName, report);
	}

	print::writelineToFile("\n\n", report);

	printSummary(authorCopiedForm, authorsCopied, vulnerabilities, matches, hashes.size(), dbProjects, authorIdToName, projectMatches, report);
	report.close();
}

void PrintMatches::parseDatabaseHashes(std::vector<std::string> &dbentries,
									   std::map<std::string, std::vector<Method>> &receivedHashes,
									   std::map<std::string, int> &projectMatches,
									   std::set<std::pair<std::string, std::string>> &projectVersions,
									   std::map<std::string, int> &dbAuthors)
{
	for (std::string entry : dbentries)
	{
		if (entry == "")
		{
			continue;
		}
		std::vector<std::string> entrySplitted = Utils::split(entry, INNER_DELIMITER);

		if (entrySplitted.size() < 12)
		{
			continue;
		}

		Method method = getMethod(entrySplitted);

		if (receivedHashes.count(entrySplitted[0]) == 1)
		{
			receivedHashes[entrySplitted[0]].push_back(method);
		}
		else
		{
			receivedHashes[entrySplitted[0]] = {method};
		}

		for (int i = 12; i < 12 + method.numberOfAuthors; i++)
		{
			dbAuthors[entrySplitted[i]]++;
		}
		projectMatches[method.projectID]++;
		projectVersions.insert(std::pair<std::string, std::string>(method.projectID, method.startVersion));
		if (method.startVersion != method.endVersion)
		{
			projectVersions.insert(std::pair<std::string, std::string>(method.projectID, method.endVersion));
		}
	}
}

PrintMatches::Method PrintMatches::getMethod(std::vector<std::string> entry)
{
	Method method;
	method.hash = entry[0];
	method.projectID = entry[1];
	method.startVersion = entry[2];
	method.startVersionHash = entry[3];
	method.endVersion = entry[4];
	method.endVersionHash = entry[5];
	method.name = entry[6];
	method.file = entry[7];
	method.lineNumber = entry[8];
	method.parserVersion = entry[9];
	method.vulnCode = entry[10];
	method.numberOfAuthors = std::stoi(entry[11]);
	method.authors = std::vector(entry.begin() + 12, entry.end());
	return method;
}

void PrintMatches::getDatabaseAuthorAndProjectData(std::set<std::pair<std::string, std::string>> &projectVersions,
												   std::map<std::string, int> &dbAuthors,
												   std::map<std::string, std::vector<std::string>> &dbProjects,
												   std::map<std::string, std::vector<std::string>> &authorIdToName,
												   EnvironmentDTO *env)
{
	// Database requests.
	std::vector<std::string> authorEntries = Utils::split(DatabaseRequests::getAuthor(dbAuthors, env), ENTRY_DELIMITER);
	std::vector<std::string> projectEntries =
		Utils::split(DatabaseRequests::getProjectData(projectVersions, env), ENTRY_DELIMITER);

	// Getting the project data out of it.
	for (int i = 0; i < projectEntries.size(); i++)
	{
		auto splitted = Utils::split(projectEntries[i], INNER_DELIMITER);
		if (splitted.size() == 1)
		{
			continue;
		}
		dbProjects[splitted[0]] = splitted;
	}
	// Getting the author data out of it.
	for (int i = 0; i < authorEntries.size(); i++)
	{
		auto splitted = Utils::split(authorEntries[i], INNER_DELIMITER);
		if (splitted.size() == 1)
		{
			continue;
		}
		authorIdToName[splitted[2]] = splitted;
	}
}

void PrintMatches::printMatch(std::vector<HashData> &hashes, std::vector<Method> dbEntries,
							  std::map<HashData, std::vector<std::string>> &authors,
							  std::map<std::string, int> &authorCopiedForm, std::map<std::string, int> &authorsCopied,
							  std::vector<std::pair<HashData *, Method>> &vulnerabilities,
							  std::map<std::string, std::vector<std::string>> &dbProjects,
							  std::map<std::string, std::vector<std::string>> &authorIdToName, std::ofstream &report)
{
	print::printAndWriteToFile("\n", report);
	for (HashData hash : hashes)
	{
		print::printAndWriteToFile("Method " + hash.functionName + " in file " + hash.fileName + " line " +
									   std::to_string(hash.lineNumber) + " with hash " + hash.hash +
									   " was found in our database.",
								   report);
		print::printAndWriteToFile("Authors of local function: ", report);
		for (std::string s : authors[hash])
		{
			Utils::replace(s, INNER_DELIMITER, '\t');
			print::printAndWriteToFile(s, report);
			authorsCopied[s]++;
		}
	}

	print::printAndWriteToFile("Database matches: ", report);

	for (Method method : dbEntries)
	{
		std::string linkFile = method.file;
		Utils::replace(linkFile, '\\', '/');

		print::printAndWriteToFile("Method " + method.name + " in project " + dbProjects[method.projectID][4] + " in file " +
									   method.file + " line " + method.lineNumber + ". (" + dbProjects[method.projectID][5] +
									   "/blob/" + method.endVersionHash + "/" + linkFile + "#L" + method.lineNumber + ")",
								   report);

		if (method.vulnCode != "")
		{
			print::printAndWriteToFile("Method marked as vulnerable with code: " + method.vulnCode + "(https://nvd.nist.gov/vuln/detail/" + method.vulnCode + ").", report);
			for (HashData hash : hashes)
			{
				vulnerabilities.push_back(std::pair(&hash, method));
			}
		}

		if (method.numberOfAuthors > 0)
		{
			print::printAndWriteToFile("Authors of function found in database: ", report);

			for (int i = 0; i < method.numberOfAuthors; i++)
			{
				if (authorIdToName.count(method.authors[i]) > 0)
				{
					authorCopiedForm[method.authors[i]]++;
					print::printAndWriteToFile("\t" + authorIdToName[method.authors[i]][0] + '\t' +
												   authorIdToName[method.authors[i]][1],
											   report);
				}
			}
		}
	}
}

void PrintMatches::printSummary(std::map<std::string, int> &authorCopiedForm, std::map<std::string, int> &authorsCopied,
								std::vector<std::pair<HashData *, Method>> &vulnerabilities, int matches, int methods,
								std::map<std::string, std::vector<std::string>> &dbProjects,
								std::map<std::string, std::vector<std::string>> &authorIdToName,
								std::map<std::string, int> &projectMatches, std::ofstream &report)
{
	print::printAndWriteToFile("\nSummary:", report);
	print::printAndWriteToFile("Methods in checked project: " + std::to_string(methods), report);

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << ((float)matches * 100 / methods);
	print::printAndWriteToFile("Matches: " + std::to_string(matches) + " (" + stream.str() + "%)", report);

	if (vulnerabilities.size() > 0)
	{
		print::printAndWriteToFile("\nVulnerabilities found:", report);

		for (const std::pair<HashData *, Method> vulnerability : vulnerabilities)
		{
			print::printAndWriteToFile(
				"Method with hash " + vulnerability.first->hash + " was found to be vulnerable in " +
					dbProjects[vulnerability.second.projectID][4] + ", with code " + vulnerability.second.vulnCode +
					"(https://nvd.nist.gov/vuln/detail/" + vulnerability.second.vulnCode + ")",
				report);
		}
	}

	print::printAndWriteToFile("\nProjects found in database:", report);

	std::vector<std::tuple<int, std::string, std::string>> vprojects = {};
	for (const auto &x : projectMatches)
	{
		vprojects.push_back(std::make_tuple(x.second, dbProjects[x.first][4], dbProjects[x.first][5]));
	}

	int longestProject = 0;
	for (const auto &x : vprojects)
	{
		int length = std::get<1>(x).size();
		if (length > longestProject)
		{
			longestProject = length;
		}
	}
	// Sorting in descending order.
	std::sort(vprojects.rbegin(), vprojects.rend());

	for (const auto &x : vprojects)
	{
		print::printAndWriteToFile(
			"\t" + std::get<1>(x) +
				std::string(longestProject - std::get<1>(x).size() - std::to_string(std::get<0>(x)).size() + 5, ' ') +
				std::to_string(std::get<0>(x)) + " (" + std::get<2>(x) + ")",
			report);
	}

	std::vector<std::tuple<int, std::string, std::string>> localAuthors;
	for (const auto &x : authorsCopied)
	{
		std::vector<std::string> author = Utils::split(x.first, '\t');
		localAuthors.push_back(std::make_tuple(x.second, author[1], author[2]));
	}
	std::vector<std::tuple<int, std::string, std::string>> remoteAuthors;
	for (const auto &x : authorCopiedForm)
	{
		std::vector<std::string> author = authorIdToName[x.first];
		remoteAuthors.push_back(std::make_tuple(x.second, author[0], author[1]));
	}

	int longestAuthor = 0;
	for (const auto &x : localAuthors)
	{
		int length = std::get<1>(x).size();
		if (length > longestAuthor)
		{
			longestAuthor = length;
		}
	}
	for (const auto &x : remoteAuthors)
	{
		int length = std::get<1>(x).size();
		if (length > longestAuthor)
		{
			longestAuthor = length;
		}
	}

	std::sort(localAuthors.rbegin(), localAuthors.rend());
	std::sort(remoteAuthors.rbegin(), remoteAuthors.rend());

	print::printAndWriteToFile("\nLocal authors present in matches: ", report);
	for (auto const &x : localAuthors)
	{
		print::printAndWriteToFile(
			"\t" + std::get<1>(x) +
				std::string(longestAuthor - std::get<1>(x).size() - std::to_string(std::get<0>(x)).size() + 5, ' ') +
				std::to_string(std::get<0>(x)) +  " " + std::get<2>(x),
			report);
	}

	print::printAndWriteToFile("\nAuthors present in database matches: ", report);
	for (auto const &x : remoteAuthors)
	{
		print::printAndWriteToFile(
			"\t" + std::get<1>(x) +
				std::string(longestAuthor - std::get<1>(x).size() - std::to_string(std::get<0>(x)).size() + 5, ' ') +
				std::to_string(std::get<0>(x)) + " " + std::get<2>(x),
			report);
	}
}

std::ofstream PrintMatches::setupOutputReport(std::string url)
{
	auto projectName = Utils::split(url, '/').back();

	std::ofstream report;
	auto filename = "reports/" + projectName + ".txt";

	std::filesystem::create_directory("reports");

	report.open(filename, std::ios::trunc);

	auto header = "Matched the project at \"" + url + "\" against the database.";
	print::printAndWriteToFile(header, report);
	print::printAndWriteToFile(std::string(header.length(), '_') + '\n', report);

	return report;
}

#pragma endregion Matches
