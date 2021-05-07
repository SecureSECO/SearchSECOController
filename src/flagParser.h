/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// External includes
#include<map>
#include<string>


class FlagParser
{
public:
	/// <summary>
	/// Will parse the given config file to set the defaults for the flags and arguments.
	/// After that it will parse the command line arguments given to it.
	/// This will all be added into a map with as key the flag.
	/// </summary>
	/// <param name="path">Path to the config file, most likely config.txt</param>
	/// <param name="args">The command line arguments</param>
	/// <param name="argc">The number of arguments</param>
	/// <returns>A map with as key the flag, and as value the corresponding value</returns>
	static std::map<std::string, std::string> parse(std::string path, std::string* args, int argc);
private:
	/// <summary>
	/// Will return a map with all valid flags and their default value for a given command
	/// </summary>
	/// <param name="command">The command you want the map for</param>
	static std::map<std::string, std::string> getDefaultFlagsForCommand(std::string command);

	/// <summary>
	/// Will read out the given config file and set the flags and arguments in the map given
	/// to the value found in the config file
	/// </summary>
	/// <param name="flagArgs">The map with of which the values will be updated</param>
	/// <param name="path">The path of the config file</param>
	static void parseFile(std::map<std::string, std::string>& flagArgs, std::string path);

	/// <summary>
	/// Updates the flags and arguments given according to the arguments given.
	/// </summary>
	/// <param name="flagArgs">The map with of which the values will be updated</param>
	/// <param name="args">The command line arguments to be parsed</param>
	/// <param name="argc">The amount of arguments</param>
	/// <param name="start">The start point from where we should start parsing the arguments</param>
	static void parseFlags(std::map<std::string, std::string>& flagArgs, std::string* args, int argc, int start);

	/// <summary>
	/// Will put update a key value pair in the given flagArgs map assuming the given argument and flag
	/// are valid. If either the argument or flag given is not valid then this function will throw an error. 
	/// </summary>
	/// <param name="flagArgs">The map with of which the value will be updated</param>
	/// <param name="flag">The flag we want to update</param>
	/// <param name="argument">The argument we want to set the flag to</param>
	/// <param name="fromFile">If the pair is from a file then we don't want to throw an error if the flag is not found in the flagargs, but instead just want to return</param>
	static void sanitize(std::map<std::string, std::string>& flagArgs, std::string flag, std::string argument, bool fromFile);
};
