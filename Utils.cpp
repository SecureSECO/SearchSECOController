#include "Utils.h"
#include <algorithm>


std::string* Utils::split(std::string str, char delimiter)
{
	int pivot = str.find(delimiter);
	std::string* ret = new std::string[2]();
	ret[0] = str.substr(0, pivot);
	ret[1] = str.substr(pivot + 1);

	return ret;
}

std::string Utils::trim(std::string str, std::vector<char> delimiters)
{
	while (str.size() > 0 && Utils::contains(delimiters, str[0])) str = str.substr(1);

	// todo trimmings at the end
	return str;
}

std::string Utils::trimWhiteSpaces(std::string str)
{
	return Utils::trim(str, std::vector<char>{' ', '\t', '\n', '\r'});
}