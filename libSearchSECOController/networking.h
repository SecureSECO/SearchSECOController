/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "dto.h"

// External includes.
#include <boost/asio.hpp>


using boost::asio::ip::tcp;

class NetworkHandler
{
public:
	static NetworkHandler* createHandler();

	/// <summary>
	/// Send data to the database.
	/// </summary>
	/// <param name="data">The data to be send.</param>
	/// <param name="dataSize">The size of the data array in bytes.</param>
	void sendData(const char* data, int dataSize);

	/// <summary>
	/// Send data to the database.
	/// </summary>
	/// <param name="data">The data to be send.</param>
	void sendData(std::string data) { sendData(&(data[0]), data.length()); };

	/// <summary>
	/// Opens a connection to the database.
	/// if -1 is used for port server and port,
	/// then we will use the list of ips defined in the .env file.
	/// </summary>
	void openConnection(EnvironmentDTO *env);

	std::string receiveData();
private:

	static boost::asio::io_context ioContext;
	static std::vector<std::pair<std::string, std::string>> ips;

	/// <summary>
	/// Basic constructor which initializes the socket with the given context.
	/// </summary>
	NetworkHandler(boost::asio::io_context& io_context)
		: socket(io_context)
	{
	};

	/// <summary>
	/// Tries to establish a connection with the given server and port.
	/// </summary>
	void connect(std::string server, std::string port);

	/// <summary>
	/// Reads the ips for the database api out of the .env file.
	/// </summary>
	void readEnvFile();

	/// <summary>
	/// Parses the string that is retrieved from the database to a list of ips.
	/// </summary>
	std::vector<std::pair<std::string, std::string>> parseIPs(std::string newIPs);

	tcp::socket socket;
};
