/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

#include "Utils.h"
//#include "Parser.h"
#include <map>
#include <fstream>
#include <iostream>
#include "ProjectMetaData.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <chrono>
#include "RunCrawler.h"

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
	while (it != str.end() && std::isdigit(*it)) ++it;
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

	// TODO: very temporary hashing.
	std::string id = pmd.authorMail + pmd.authorName + pmd.version;
	long long hash = 0;
	for (int i = 0; i < id.size(); i++)
	{
		hash += id[i] *(i+1);
	}

	return ProjectMetaData(std::to_string(hash),
		std::to_string(getIntegerTimeFromString(pmd.version)), 
		pmd.license, 
		pmd.name, 
		pmd.url, 
		pmd.authorName, 
		pmd.authorMail);
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
	std::tm epoch_start = {};
	epoch_start.tm_sec = 0;
	epoch_start.tm_min = 0;
	epoch_start.tm_hour = 0;
	epoch_start.tm_mday = 1;
	epoch_start.tm_mon = 0;
	epoch_start.tm_year = 70;
	epoch_start.tm_wday = 4;
	epoch_start.tm_yday = 0;
	epoch_start.tm_isdst = -1;

	std::time_t base = std::mktime(&epoch_start);
	auto diff = std::chrono::system_clock::from_time_t(version) - std::chrono::system_clock::from_time_t(base);
	std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
	return s.count();
}
#pragma endregion utils
