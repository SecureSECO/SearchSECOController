/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "print.h"
#include "utils.h"

// Crawler includes.
#include "ProjectMetadata.h"

// Parser includes.
#include "md5/md5.h"

// External includes.
#include <filesystem>
#include <thread>
#include <sstream>

// OS Dependent includes.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// Windows.
#include <Windows.h>
#else
// Unix.
#include <unistd.h>
#include <linux/limits.h>
#endif


std::vector<std::string> Utils::split(std::string str, char delimiter)
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

std::string Utils::trim(std::string str, std::string delimiters)
{
	str.erase(0, str.find_first_not_of(delimiters));
	str.erase(str.find_last_not_of(delimiters)+1);
	return str;
}

std::string Utils::trimWhiteSpaces(std::string str)
{
	return Utils::trim(str, " \t\n\r");
}

bool Utils::isNumber(std::string str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it))
	{
		++it;
	}
	return !str.empty() && it == str.end();
}

std::string Utils::padLeft(std::string src, char pad, int length)

{
	while(src.length() < length)
	{
		src = pad + src;
	}

	return src;
}

long long Utils::getIntegerTimeFromString(std::string time)
{
	// Somehow, sleeping for 0 milliseconds fixed this conversion. Without this sleep, the integration test fails.
	std::this_thread::sleep_for(std::chrono::milliseconds(0));


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
	epochStart.tm_mon = 1;
	epochStart.tm_year = 70;
	epochStart.tm_wday = 4;
	epochStart.tm_yday = 0;
	epochStart.tm_isdst = -1;

	std::time_t base = std::mktime(&epochStart);
	auto diff = std::chrono::system_clock::from_time_t(version) - std::chrono::system_clock::from_time_t(base);
	std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
	return s.count();
}
void Utils::replace(std::string& string, char replace, char with)
{
	for (int i = 0; i < string.size(); i++)
	{
		if (string[i] == replace)
		{
			string[i] = with;
		}
	}
}

std::string Utils::getExecutablePath()
{
	// Code borrowed from https://stackoverflow.com/a/1528493,
	// https://stackoverflow.com/questions/18783087/how-to-properly-use-getmodulefilename, and
	// https://stackoverflow.com/questions/23943239/how-to-get-path-to-current-exe-file-on-linux.

	std::filesystem::path path;

	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// Windows.
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring ws(buffer);
	path = std::filesystem::path(std::string(ws.begin(), ws.end()));
	#else
	// Unix.
	char buffer[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
	path = std::filesystem::path(std::string(buffer, (count > 0) ? count : 0));
	#endif

	auto str = path
		.parent_path()
		.string();

	print::debug("Found executable path as " + print::quote(str), __FILE__, __LINE__);
	
	return str;
}

long long Utils::getIdFromPMD(ProjectMetadata pmd)
{

	std::string id = pmd.name + pmd.authorName + pmd.url;
	std::string md5hash = md5(id);
	long long hash = 0;
	for (int i = 0; i < 16; i++)
	{
		int num = 0;
		switch (md5hash[i])
		{
		case 'a':
			num = 10;
			break;
		case 'b':
			num = 11;
			break;
		case 'c':
			num = 12;
			break;
		case 'd':
			num = 13;
			break;
		case 'e':
			num = 14;
			break;
		case 'f':
			num = 15;
			break;
		default:
			num = std::stoi(std::string(1, md5hash[i]));
		}
		hash += num << (i * 4);
	}
	if (hash < 0)
	{
		hash = -hash;
	}
	return hash;
}

#pragma endregion utils
