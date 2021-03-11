#include "Utils.h"
#include <algorithm>


std::string* utils::split(std::string str, char delimiter)
{
	int pivot = str.find(delimiter);
	std::string* ret = new std::string[2]();
	ret[0] = str.substr(0, pivot);
	ret[1] = str.substr(pivot + 1);

	return ret;
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

bool utils::is_number(std::string str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it)) ++it;
	return !str.empty() && it == str.end();
}

std::string utils::pad_left(std::string src, char pad, int length)

{
	while(src.length() < length)
	{
		src = pad + src;
	}

	return src;
}