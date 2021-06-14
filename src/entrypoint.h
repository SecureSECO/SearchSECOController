/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

#include <string>

#define DATABASE_API_IP "131.211.31.153"
#define DATABASE_API_PORT "8003"



namespace entrypoint 
{
	void entrypoint(int argc, char* argv[], std::string apiIP = DATABASE_API_IP, std::string apiPort = DATABASE_API_PORT);
}
