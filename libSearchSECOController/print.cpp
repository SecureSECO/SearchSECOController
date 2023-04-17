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
#include <algorithm>

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
									EnvironmentDTO *env, std::string url, std::string projectID, std::string projectLicense, std::string startVersion)
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
	std::string matchesReport;
	//removed const here
	int numberOfConflcits = 0;
	for (auto &match : receivedHashes)
	{
		if (std::count_if(match.second.begin(), match.second.end(), [projectID](Method m) { return m.projectID != projectID; }) > 0)
		{
			matches++;

			// Function to check for license confclits
			// method.second is a vector of methods
			// Checking every method's license for its compatibility with projectLicense

			numberOfConflcits+=checkLicenseConflicts(match.second,projectLicense,startVersion);
			
			printMatch(hashMethods[match.first], match.second, authors, projectID, authorCopiedForm, authorsCopied,
					   vulnerabilities, dbProjects, authorIdToName, matchesReport);
		}
	}

	printSummary(authorCopiedForm, authorsCopied, vulnerabilities, matches, hashes.size(), dbProjects, authorIdToName, projectMatches, report,numberOfConflcits);

	print::printAndWriteToFile("\n\n" + std::string(128, '-'), report);
	print::printAndWriteToFile("Details of matches found", report);
	print::printAndWriteToFile(std::string(128, '-'), report);
	print::printAndWriteToFile(matchesReport, report);

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

		for (int i = 13; i < 13 + method.numberOfAuthors; i++)
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
	method.license = entry[11];
	method.numberOfAuthors = std::stoi(entry[12]);
	method.authors = std::vector(entry.begin() + 13, entry.end());
	method.licenseConflict = false;
	method.licenseConflictMessage = "";
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
							  std::map<HashData, std::vector<std::string>> &authors, std::string projectID,
							  std::map<std::string, int> &authorCopiedForm, std::map<std::string, int> &authorsCopied,
							  std::vector<std::pair<HashData *, Method>> &vulnerabilities,
							  std::map<std::string, std::vector<std::string>> &dbProjects,
							  std::map<std::string, std::vector<std::string>> &authorIdToName, std::string &report)
{
	std::string header = "Hash " + hashes[0].hash;
	report += std::string(header.length(), '-') + "\n";
	report += header + "\n";
	report += std::string(header.length(), '-') + "\n\n";

	for (HashData hash : hashes)
	{
		report += "  *Method " + hash.functionName + " in file " + hash.fileName + " line " +
				  std::to_string(hash.lineNumber) + "\n";
		report += "   Authors of local function: \n";
		for (std::string s : authors[hash])
		{
			Utils::replace(s, INNER_DELIMITER, '\t');
			report += "  " + s + "\n";
			authorsCopied[s]++;
		}
		report += "\n";
	}

	report += "DATABASE\n";

	for (Method method : dbEntries)
	{
		if (method.projectID == projectID)
		{
			continue;
		}
		
		// IMPORTANT: do not use dbProjects[method.projectID] to check for existence, as this adds a new map entry
		// https://cplusplus.com/reference/map/map/operator[]/
		if (dbProjects.count(method.projectID) == 0)
		{
			// ensure projectID is within dbProjects, if not...
			continue;
		}

		//TD provide error-handling and reporting
		// check if data is available to avoid out-of-bounds access to dbProjects
		if (dbProjects[method.projectID].size() < 4)
		{
			print::printline("ERR1: projectID " + method.projectID + " has missing dbProjects data");
			continue;
		}

		std::string linkFile = method.file;
		Utils::replace(linkFile, '\\', '/');

		report += "  *Method " + method.name + " in project " + dbProjects[method.projectID][4] + " in file " +
				  method.file + " line " + method.lineNumber + "\n";
		report += "  URL: " + dbProjects[method.projectID][5] + "/blob/" + method.endVersionHash + "/" + linkFile +
				  "#L" + method.lineNumber + "\n";

		if (method.vulnCode != "")
		{
			report += "  Method marked as vulnerable with code: " + method.vulnCode +
					  "(https://nvd.nist.gov/vuln/detail/" + method.vulnCode + ").\n";
			for (HashData hash : hashes)
			{
				vulnerabilities.push_back(std::pair(&hash, method));
			}
		}

		if (method.numberOfAuthors > 0)
		{
			report += "  Authors of function found in database: \n";

			for (int i = 0; i < method.numberOfAuthors; i++)
			{
				if (authorIdToName.count(method.authors[i]) > 0)
				{
					authorCopiedForm[method.authors[i]]++;
					report += "  \t" + authorIdToName[method.authors[i]][0] + '\t' +
							  authorIdToName[method.authors[i]][1] + "\n";
				}
			}
		}

		if (method.licenseConflict)
		{
			report += "\n  License conflict found: \n" + method.licenseConflictMessage + "\n";
		}
		else
		{
			report += "\n  No license conflict found.\n";
		}

		report += "\n";
	}
}

PrintMatches::licenseType PrintMatches::convertToLicenseType(const std::string& licenseString) 
{
	//convert license string to lower case
	std::string licenseLower = licenseString;
	std::transform(licenseLower.begin(), licenseLower.end(), licenseLower.begin(), ::tolower);
	
    static const std::map<std::string, PrintMatches::licenseType> licenseMap = 
	{
        {"creative commons license family", PrintMatches::PUBLIC_DOMAIN},
		{"do what the f*ck you want to public license", PrintMatches::PUBLIC_DOMAIN},
		{"academic free license v3.0", PrintMatches::PERMISSIVE},
		{"apache license 2.0", PrintMatches::PERMISSIVE},
		{"artistic license 2.0", PrintMatches::WEAKLY_PROTECTIVE},
		{"the unlicense", PrintMatches::PERMISSIVE},
		{"boost software license 1.0", PrintMatches::PERMISSIVE},
		{"bsd 2-clause \"simplified\" license", PrintMatches::PERMISSIVE},
		{"bsd 3-clause \"new\" or \"revised\" license", PrintMatches::PERMISSIVE},
		{"bsd 3-clause clear license", PrintMatches::PERMISSIVE},
		{"creative commons zero v1.0 universal", PrintMatches::PUBLIC_DOMAIN},
		{"eclipse public license 1.0", PrintMatches::WEAKLY_PROTECTIVE},
		{"mit license", PrintMatches::PERMISSIVE},
		{"mozilla public license 2.0", PrintMatches::WEAKLY_PROTECTIVE},
		{"common development and distribution license 1.0", PrintMatches::WEAKLY_PROTECTIVE},\
		//There is debate if it is weakly copyleft or permissve
		{"microsoft public license", PrintMatches::WEAKLY_PROTECTIVE},
		{"gnu general public license v2.0", PrintMatches::STRONGLY_PROTECTIVE},
		{"gnu general public license v3.0", PrintMatches::STRONGLY_PROTECTIVE},
		{"gnu lesser general public license v2.1", PrintMatches::STRONGLY_PROTECTIVE},
		{"gnu lesser general public license v3.0", PrintMatches::STRONGLY_PROTECTIVE},
		{"affero general public license v3.0", PrintMatches::STRONGLY_PROTECTIVE},
    };

    auto it = licenseMap.find(licenseLower);
    if (it != licenseMap.end()) 
	{
        return it->second;
    } 
	else 
	{
        return PrintMatches::UNKNOWN;
    }
}

int PrintMatches::checkLicenseConflicts(std::vector<Method>& methods,std::string projectLicense, std::string startVersion)
{
	std::map<PrintMatches::Method,std::pair<bool,std::string>> licenseCheckResult;
	int numberofConflicts = 0;

	//loop through the methods
	for (Method &method : methods)
	{
		bool conflictFound = false;
    	std::string conflictMessage;
		
		//check if the method has a license
		if (method.license.size() > 0)
		{
			licenseType myLicenseType = convertToLicenseType(projectLicense);

			//if method.startVersion < startVersion, aka method was created before the current project
			//aka this project should be inheriting that method's license restrictions
			if (method.startVersion < startVersion)
			{
    			licenseType borrowedLicenseType = convertToLicenseType(method.license);

				//if the license of the method is unknown, then we can't check for conflicts
				if (borrowedLicenseType == UNKNOWN || myLicenseType == UNKNOWN)
				{
					conflictMessage = "\n  Unknown license type. Unable to check for conflicts.";
				}
				else
				{
					//Switch on the license type of the method
					switch (borrowedLicenseType)
					{
						case PUBLIC_DOMAIN: 
						
						conflictMessage = "\n  Conflict Severity: No conflict\n  Reason: Method was found in a project with Public License.";
						break;

						case PERMISSIVE:

						conflictMessage = "\n  Conflict Severity: No conflict\n  Reason: Method was found in a project with Permissive License. Ensure that the original license is included in your software";
						break;
						
						case WEAKLY_PROTECTIVE:

						if (myLicenseType == PUBLIC_DOMAIN || myLicenseType == PERMISSIVE)
						{
							conflictFound = true;
							conflictMessage = "\n  Conflict severity: Medium\n  Reason: Repository being checked is less restrictive than the license of the matched method's project. The license for the part of borrowed method has to be under "+method.license;
						}
						else
						{
							if (method.license == projectLicense)
							{
								conflictMessage = "\n  Conflict Severity: No conflict\n  Reason: Repository being checked is compatible with the license of the matched method.";
							}
							else
							{
								conflictFound = true;
								conflictMessage = "\n  Conflict severity: Medium\n  Reason: Repository being checked does not use the same weak copy-left license as the matched method. The license for the part of borrowed method has to be under "+method.license;
							}		
						}
						break;

						case STRONGLY_PROTECTIVE:
						// Conflict if borrowed code is strongly protective and your project is not
						if (myLicenseType == PUBLIC_DOMAIN || myLicenseType == PERMISSIVE || myLicenseType == WEAKLY_PROTECTIVE)
						{
							conflictFound = true;
							conflictMessage = "\n  Conflict Severity: High\n  Reason: Repository being checked has license - "+projectLicense+", which is less restrictive than the license of the matched method's project with license - "+method.license;
						}
						else
						{
							if (method.license == projectLicense)
							{
								conflictMessage = "\n  Conflict Severity: No conflict\n  Reason: Repository being checked is compatible with the license of the matched method.";
							}
							else
							{
								conflictFound = true;
								conflictMessage = "\n  Conflict Severity: High\n  Reason: Repository being checked does not use the same strong copy-left license as the matched method. Please verify if the licenses are compatible.";
							}
						}
						break;

						case UNKNOWN:

						conflictMessage = "\n  Conflict Severity: Unknown\n  Reason: License of matched method wasn't found.";
						break;
						
						default:
						conflictMessage = "\n  Conflict Severity: Unknown\n  Reason: Unknown";
						break;
					}
				}
			}
			//if method.startVersion > startVersion, aka method was created after the current project
			//aka that method should be inheriting this project's license restrictions
			else
			{
				licenseType borrowingLicenseType = convertToLicenseType(method.license);
				if (borrowingLicenseType == UNKNOWN || myLicenseType == UNKNOWN)
				{
					//handle this differently
					conflictMessage = "\n  Unknown license type. Unable to check for conflicts.";
				}
				else
				{
					switch (myLicenseType)
					{
						case PUBLIC_DOMAIN: 
						
						conflictMessage = "\n  Conflict Severity: No conflict\n  Reason: Method was found in a project with Public License.";
						break;

						case PERMISSIVE:

						conflictMessage = "\n  Conflict Severity: No conflict\n  Reason: Repository has permissive license. Ensure that the your license is included in the matched method's repository.";
						break;
						
						case WEAKLY_PROTECTIVE:

						if (borrowingLicenseType == PUBLIC_DOMAIN || borrowingLicenseType == PERMISSIVE)
						{
							conflictFound = true;
							conflictMessage = "\n  Conflict Severity: Medium\n  Reason: Matched method's repository has a less restrictive than your license.";
						}
						else
						{
							if (method.license == projectLicense)
							{
								conflictMessage = "\n  Conflict Severity: No conflict\n  Reason: Matched repository is compatible with the license of your repository.";
							}
							else
							{
								conflictFound = true;
								conflictMessage = "\n  Conflict Severity: Medium\n  Reason: Repository being checked does not use the same weak copy-left license as the matched method. The license for the part of borrowed method has to be under "+projectLicense;
							}		
						}
						break;

						case STRONGLY_PROTECTIVE:
						// Conflict if borrowed code is strongly protective and your project is not
						if (borrowingLicenseType == PUBLIC_DOMAIN || borrowingLicenseType == PERMISSIVE || borrowingLicenseType == WEAKLY_PROTECTIVE)
						{
							conflictFound = true;
							conflictMessage = "\n  Conflict Severity: High\n  Reason: Matched method is less restrictive than the license of the your repository.";
						}
						else
						{
							if (method.license == projectLicense)
							{
								conflictMessage = "\n  Conflict Severity: No conflict\n  Reason: Repository being checked is compatible with the license of the matched method.";
							}
							else
							{
								conflictFound = true;
								conflictMessage = "\n  Conflict Severity: High\n  Reason: Matched method does not use the same copy-left license as your project. Contact the author of the matched method to solve the conflict.";
							}
						}
						break;

						case UNKNOWN:

						conflictMessage = "\n  Conflict Severity: Unknown\n  Reason: License of matched method wasn't found.";
						break;
						
						default:
						conflictMessage = "\n  Conflict Severity: Unknown\n  Reason:Unknown";
						break;
					}

				}
			}

			if(conflictFound)
			{
				++numberofConflicts;
			}
			method.licenseConflict = conflictFound;
			method.licenseConflictMessage = conflictMessage;

		}
		else
		{
			print::printline("Error because license does not exist.");
		}
	}
	
	return numberofConflicts;
	
}

void PrintMatches::printSummary(std::map<std::string, int> &authorCopiedForm, std::map<std::string, int> &authorsCopied,
								std::vector<std::pair<HashData *, Method>> &vulnerabilities, int matches, int methods,
								std::map<std::string, std::vector<std::string>> &dbProjects,
								std::map<std::string, std::vector<std::string>> &authorIdToName,
								std::map<std::string, int> &projectMatches, std::ofstream &report, int numberOfConflicts)
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
				"Method with hash " + vulnerability.second.hash + " was found to be vulnerable in " +
					dbProjects[vulnerability.second.projectID][4] + ", with code " + vulnerability.second.vulnCode +
					"(https://nvd.nist.gov/vuln/detail/" + vulnerability.second.vulnCode + ")",
				report);
		}
	}

	if (numberOfConflicts > 0)
	{
		print::printAndWriteToFile("\nNumber of license conflicts found: " + std::to_string(numberOfConflicts), report);
	}
	else
	{
		print::printAndWriteToFile("\nLicense conflicts were checked and no license conflicts found.", report);
	}
	print::printAndWriteToFile("\nProjects found in database:", report);

	std::vector<std::tuple<int, std::string, std::string>> vprojects = {};
	for (const auto &x : projectMatches)
	{
		// IMPORTANT: do not use dbProjects[method.projectID] to check for existence, as this adds a new map entry
		// https://cplusplus.com/reference/map/map/operator[]/
		if (dbProjects.count(x.first) == 0)
		{
			// ensure projectID is within dbProjects, if not...
			continue;
		}

		// check if data is available to avoid out-of-bounds access to dbProjects
		// TD provide error-handling and reporting
		if (dbProjects[x.first].size() < 4)
		{
		    print::printline("ERR2: projectID " + x.first + " has missing dbProjects data");
			continue;
		}

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
	print::printAndWriteToFile(std::string(header.length(), '-'), report);
	print::printAndWriteToFile(header, report);
	print::printAndWriteToFile(std::string(header.length(), '-') + '\n', report);

	return report;
}

#pragma endregion Matches
