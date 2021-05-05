/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#pragma once
#include <boost/asio.hpp>
#include "../parser/Parser/HashData.h"

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
	/// </summary>
	void openConnection(std::string server, std::string port);

	std::string receiveData();
private:
	static boost::asio::io_context ioContext;
	/// <summary>
	/// Basic constructor which initializes the socket with the given context.
	/// </summary>
	NetworkHandler(boost::asio::io_context& io_context)
		: socket(io_context)
	{
	};

	tcp::socket socket;
};
