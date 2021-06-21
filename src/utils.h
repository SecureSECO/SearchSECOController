/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// External includes
#include <algorithm>
#include <map>
#include <string>
#include <vector>


struct ProjectMetaData;
struct ProjectMetadata;

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
	/// Converts a yyyy:mm::dd hh:mm:ss format to a long long.
	/// </summary>
	static long long getIntegerTimeFromString(std::string time);

	/// <summary>
	/// Replaces each occurence of the replace char with the with char
	/// in the given string.
	/// </summary>
	/// <param name="string"></param>
	/// <param name="replace"></param>
	/// <param name="with"></param>
	static void replace(std::string& string, char replace, char with);

	/// <summary>
	/// Gets the path to the running executable.
	/// </summary>
	static std::string getExecutablePath();

	/// <summary>
	/// Randomly shuffles a list.
	/// </summary>
	template <class C>
	static void shuffle(std::vector<C> &list) 
	{
		int size = list.size();
		for (int i = 0; i < size - 1; i++)
		{
			int r = i + (rand() % (size - i));
			C temp = list[r];
			list[r] = list[i];
			list[i] = temp;
		}
	};

	/// <summary>
	/// Generates a project id for the given project meta data using md5.
	/// </summary>
	static long long getIdFromPMD(ProjectMetadata pmd);
};
