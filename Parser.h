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
	/// to the value found in the config file.
	/// </summary>
	/// <param name="flagArgs">The map with of which the values will be updated.</param>
	/// <param name="path">The path of the config file.</param>
	static std::map<std::string, std::string> parseConfig(std::string configPath);

};
