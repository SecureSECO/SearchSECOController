/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// External includes.
#include <string>

class EnvironmentDTO 
{
public:
	std::string
		databaseAPIIP,
		databaseAPIPort,
		commandString;

	/// <summary>
	/// Constructs the DTO. Requires the database IP and Port, and a string
	/// representing the currently executing command.
	/// </summary>
	EnvironmentDTO(std::string ip, std::string port, std::string command)
	{
		this->databaseAPIIP = ip;
		this->databaseAPIPort = port;
		this->commandString = command;
	}
};
