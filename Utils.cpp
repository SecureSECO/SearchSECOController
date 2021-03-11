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