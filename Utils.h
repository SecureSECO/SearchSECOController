#pragma once
#include <string>
#include <vector>

namespace Utils
{
	template <typename T>
	static bool contains(std::vector<T> ts, T x)
	{
		return std::find(ts.begin(), ts.end(), x) != ts.end();
	}

	std::string* split(std::string str, char delimiter);
	std::string trim(std::string str, std::string delimiters);
	std::string trimWhiteSpaces(std::string str);
};



