/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct ProjectMetaData;

class utils
{
public:

	/// <summary>
	/// check if x exists in ts.
	/// </summary>
	template <class C, typename T>
	static bool contains(C&& ts, T x)
	{
		return std::find(ts.begin(), ts.end(), x) != ts.end();
	}

	/// <summary>
	/// check if x exists in ts.
	/// </summary>
	template <class C, typename T>
	static bool contains(C&& ts, T x, int cnt)
	{
		for (int i = 0; i < cnt; ++i)
		{
			if (ts[i] == x) return true;
		}
		return false;
	}

	/// <summary>
	/// splits a string on a given delimiter.
	/// </summary>
	static std::vector<std::string> split(std::string str, char delimiter);

	/// <summary>
	/// trims a string on the front and the back on all characters in delimiters.
	/// </summary>
	static std::string trim(std::string str, std::string delimiters);

	/// <summary>
	/// uses trim to trim spaces, tabs and newlines.
	/// </summary>
	static std::string trimWhiteSpaces(std::string str);

	/// <summary>
	/// Checks if the input string is a number or not.
	/// </summary>
	static bool isNumber(std::string str);

	/// <summary>
	/// Pads a string with a given char untill the string is the requested length.
	/// </summary>
	static std::string padLeft(std::string src, char pad_left, int length);

	/// <summary>
	/// Retrieves the project meta data from the file you give it.
	/// </summary>
	static ProjectMetaData getProjectMetaDataFromFile(std::string file);

	static long long getIntegerTimeFromString(std::string time);
};

