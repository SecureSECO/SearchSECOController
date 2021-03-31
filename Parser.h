/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

#include<string>
#include<map>

// TODO namespace?
class FlagParser
{
public:
	/// <summary>
	/// Will read out the given config file and set the flags and arguments in the map given
	/// to the value found in the config file
	/// </summary>
	/// <param name="flagArgs">The map with of which the values will be updated</param>
	/// <param name="path">The path of the config file</param>
	static std::map<std::string, std::string> parseConfig(std::string configPath);

	/// <summary>
	/// Will put update a key value pair in the given flagArgs map assuming the given argument and flag
	/// are valid. If either the argument or flag given is not valid then this function will throw an error. 
	/// </summary>
	/// <param name="flagArgs">The map with of which the value will be updated</param>
	/// <param name="flag">The flag we want to update</param>
	/// <param name="argument">The argument we want to set the flag to</param>
	/// <param name="fromFile">If the pair is from a file then we don't want to throw an error if the flag is not found in the flagargs, but instead just want to return</param>
	static void sanitize(std::string flag, std::string argument, bool fromFile);
};
