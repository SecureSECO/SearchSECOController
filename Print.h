/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

#include<string>

namespace print
{
	/// <summary>
	/// Prints a given string to the console, followed by a newline character.
	/// </summary>
	/// <param name="str">The string to output.</param>
	void printline(std::string str);

	/// <summary>
	/// Returns a string of n tabs.
	/// </summary>
	std::string tab(int n = 1);

	/// <summary>
	/// Prints the help message to the console.
	/// </summary>
	void help();

	/// <summary>
	/// Prints the help message to the console, with an additional message.
	/// </summary>
	/// <param name="str">An additional message to print before the help message.</param>
	void help(std::string str);

	/// <summary>
	/// Encapsulates a string with a single character; so once before the string, and once behind it.
	/// </summary>
	/// <param name="str">The base string to be encapsulated.</param>
	/// <param name="c">The character to encapsulate with.</param>
	/// <returns>A string which is c + str + c.</returns>
	std::string encapsulate(std::string str, char c);

	/// <summary>
	/// Puts a string in quotation marks.
	/// </summary>
	/// <param name="str">The string to put inside quotation marks.</param>
	/// <returns>A quoted string, e.g. "string"</returns>
	std::string quote(std::string str);

	/// <summary>
	/// Encapsulates str with spaces and q with quotation marks, then concatenates them.
	/// </summary>
	/// <param name="str">The string to be encapsulated with spaces.</param>
	/// <param name="q">The string to be put inside quotation marks.</param>
	/// <returns>A string of the format: { str "q"}</returns>
	std::string text_then_quote(std::string str, std::string q);

	/// <summary>
	/// Prints the Controller version, and the versions of the subsystems to the console.
	/// </summary>
	void version_full();

}

namespace error 
{
	/// <summary>
	/// Logs a string to the console.
	/// </summary>
	void log(std::string str);

	/// <summary>
	/// Prints the warning message with the supplied warning code to the console.
	/// </summary>
	/// <param name="code">The code of the warning which should be displayed.</param>
	void warn(int code);

	/// <summary>
	/// Throws the "Insufficient Arguments" error for a flag or command.
	/// </summary>
	/// <param name="command">Name of the command which has insufficient arguments.</param>
	void err_insufficient_arguments(std::string command);

	/// <summary>
	/// Throws the "Invalid Command" error.
	/// </summary>
	void err_invalid_command(std::string command);

	/// <summary>
	/// Throws the "Flag Does Not Exist" error.
	/// </summary>
	/// <param name="flag">The flag that does not exist.</param>
	/// <param name="from_config">Whether this flag appeared in the config file (true) or was entered in the command line (false).</param>
	void err_flag_not_exist(std::string flag, bool from_config);

	/// <summary>
	/// Throws the "Invalid Argument" error for flags.
	/// </summary>
	/// <param name="flag">The flag that had an invalid argument.</param>
	/// <param name="arg">The (invalid) argument.</param>
	/// <param name="from_config">Whether this flag appeared in the config file (true) or was entered in the command line (false).</param>
	void err_flag_invalid_arg(std::string flag, std::string arg, bool from_config);

	/// <summary>
	/// Throws the "Command Not Found" error.
	/// </summary>
	void err_cmd_not_found();

	/// <summary>
	/// Throws the "Command Does Not Exist" error.
	/// </summary>
	/// <param name="command">The command that does not exist.</param>
	void err_cmd_not_exist(std::string command);

	/// <summary>
	/// Throws the "Not Implemented" error.
	/// </summary>
	void err_not_implemented(std::string message);
}
