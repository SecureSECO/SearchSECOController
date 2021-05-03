/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#include "Networking.h"
#include <boost/array.hpp>
#include <iostream>

#include "Print.h"

// https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio/tutorial.html was used as a base.
boost::asio::io_context NetworkHandler::ioContext;

void NetworkHandler::openConnection(std::string server, std::string port)
{
	std::string serverApi = server;

	tcp::resolver resolver(ioContext);
	tcp::resolver::results_type endpoints = resolver.resolve(serverApi, port);

	boost::asio::connect(socket, endpoints);
}

NetworkHandler* NetworkHandler::createHandler()
{
	return new NetworkHandler(ioContext);
}

void NetworkHandler::sendData(const char* data, int dataLength)
{
	print::printline("Sending " + std::to_string(dataLength) + " bytes");
	boost::asio::write(socket, boost::asio::buffer(data, dataLength));
}

std::string NetworkHandler::receiveData()
{
	// The buffer we are going to return as a string.
	std::vector<char> ret = std::vector<char>();
	for (;;)
	{

		boost::array<char, 128> buf;
		boost::system::error_code error;

		// Read incomming data.
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		// Add it to out buffer.
		for (int i = 0; i < len; i++)
		{
			ret.push_back(buf[i]);
		}

		if (error == boost::asio::error::eof)
		{
			break; // Connection closed cleanly by peer.
		}
		else if (error)
		{
			// If any error occures, we just throw.
			std::cout << "Networking error: " << error.message();
			throw boost::system::system_error(error);
		}

	}
	return std::string(ret.begin(), ret.end());
}

