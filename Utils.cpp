/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Utils.h"
#include <algorithm>
#include <map>
#include <fstream>
#include <iostream>
#include "ProjectMetaData.h"

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

ProjectMetaData utils::getProjectMetaDataFromFile(std::string file)
{
	// TODO: handle files that are not what we expect
	std::map<std::string, std::string> entries;

	std::ifstream configFile(file);
	std::string line;

	while (std::getline(configFile, line))
	{
		std::vector<std::string> splitted = split(line, ':');
		entries[splitted[0]] = splitted[1];
	}
	return ProjectMetaData("3db1a3fc-24d8-4017-8d8b-7a2673625c58", entries["updated at"], entries["license"], split(entries["name"], '/')[1], entries["url"], split(entries["name"], '/')[0], entries["email"], "4");
}
