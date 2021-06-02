/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "projectMetadata.h"
#include "utils.h"

// Crawler includes
#include "RunCrawler.h"

// External includes
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>


std::vector<std::string> utils::split(std::string str, char delimiter)
{
	int pivot = str.find(delimiter);
	std::string rest = str;
	std::vector<std::string> out;
	while (pivot != std::string::npos)
	{
		std::string* ret = new std::string[2]();
		out.push_back(rest.substr(0, pivot));
		rest = rest.substr(pivot + 1);
		pivot = rest.find(delimiter);
	}
	out.push_back(rest);
	return out;
}

std::string utils::trim(std::string str, std::string delimiters)
{
	str.erase(0, str.find_first_not_of(delimiters));
	str.erase(str.find_last_not_of(delimiters)+1);
	return str;
}

std::string utils::trimWhiteSpaces(std::string str)
{
	return utils::trim(str, " \t\n\r");
}

bool utils::isNumber(std::string str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it))
	{
		++it;
	}
	return !str.empty() && it == str.end();
}

std::string utils::padLeft(std::string src, char pad, int length)

{
	while(src.length() < length)
	{
		src = pad + src;
	}

	return src;
}

ProjectMetaData utils::getProjectMetadata(std::string url)
{
	ProjectMetadata pmd = RunCrawler::findMetadata(url);
	int er = errno;
	// TODO: very temporary hashing.
	std::string id = pmd.authorMail + pmd.authorName + pmd.version;
	long long hash = 0;
	for (int i = 0; i < id.size(); i++)
	{
		hash += id[i] *(i+1);
	}
	ProjectMetaData pm = ProjectMetaData(std::to_string(hash),
		std::to_string(getIntegerTimeFromString(pmd.version)),
		pmd.license,
		pmd.name,
		pmd.url,
		pmd.authorName,
		pmd.authorMail,
		pmd.defaultBranch);
	errno = er;
	return pm;
}

long long utils::getIntegerTimeFromString(std::string time)
{
	// Used https://stackoverflow.com/questions/4781852/how-to-convert-a-string-to-datetime-in-c for this convertion.
	static const std::string dateTimeFormat{ "%Y-%m-%dT%H:%M:%SZ" };
	std::stringstream ss{ time };
	std::tm dt;
	ss >> std::get_time(&dt, dateTimeFormat.c_str());
	std::time_t version = std::mktime(&dt);

	// Used https://stackoverflow.com/questions/9483974/converting-time-t-to-int for this part.
	std::tm epochStart = {};
	epochStart.tm_sec = 0;
	epochStart.tm_min = 0;
	epochStart.tm_hour = 0;
	epochStart.tm_mday = 1;
	epochStart.tm_mon = 0;
	epochStart.tm_year = 70;
	epochStart.tm_wday = 4;
	epochStart.tm_yday = 0;
	epochStart.tm_isdst = -1;

	std::time_t base = std::mktime(&epochStart);
	auto diff = std::chrono::system_clock::from_time_t(version) - std::chrono::system_clock::from_time_t(base);
	std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
	return s.count();
}
void utils::replace(std::string& string, char replace, char with)
{
	for (int i = 0; i < string.size(); i++)
	{
		if (string[i] == replace)
		{
			string[i] = with;
		}
	}

}
#pragma endregion utils
