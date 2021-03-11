#pragma once
#include <string>
#include <vector>

class utils
{
public:
	template <typename T>
	static bool contains(std::vector<T> ts, T x)
	{
		return std::find(ts.begin(), ts.end(), x) != ts.end();
	}
	template <typename T>
	static bool contains(T* ts, T x, int cnt)
	{
		for (int i = 0; i < cnt; ++i)
		{
			if (ts[i] == x) return true;
		}
		return false;
	}

	static std::string* split(std::string str, char delimiter);
	static std::string trim(std::string str, std::string delimiters);
	static std::string trimWhiteSpaces(std::string str);
	static bool is_number(std::string str);

	static std::string pad_left(std::string src, char pad_left, int length);
};



