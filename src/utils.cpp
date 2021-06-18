/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "print.h"
#include "projectMetadata.h"
#include "utils.h"

// External includes
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

// OS Dependent includes
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// Windows
#include <Windows.h>
#else
// Unix
#include <unistd.h>
#include <linux/limits.h>
#endif


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

std::string utils::getExecutablePath()
{
	// Code borrowed from https://stackoverflow.com/a/1528493,
	// https://stackoverflow.com/questions/18783087/how-to-properly-use-getmodulefilename, and
	// https://stackoverflow.com/questions/23943239/how-to-get-path-to-current-exe-file-on-linux

	std::string path;

	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// Windows
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring ws(buffer);
	path = std::filesystem::path(std::string(ws.begin(), ws.end()))
		.parent_path()
		.string();
	#else
	// Unix
	char buffer[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
	path = std::filesystem::path(std::string(buffer, (count > 0) ? count : 0))
		.parent_path()
		.string();
	#endif

	print::debug("Found executable path as " + print::quote(path), __FILE__, __LINE__);
	
	return path;
}

#pragma endregion utils
