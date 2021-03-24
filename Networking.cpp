#include "Networking.h"
#include <boost/array.hpp>
#include <iostream>

// Made using this tutorial https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio/tutorial.html
boost::asio::io_context NetworkHandler::io_context;

void NetworkHandler::openConnection(std::string server, std::string port)
{
	std::string serverApi = server;

	tcp::resolver resolver(io_context);
	tcp::resolver::results_type endpoints = resolver.resolve(serverApi, port);

	boost::asio::connect(socket, endpoints);
}

NetworkHandler* NetworkHandler::createHandler()
{
	return new NetworkHandler(io_context);
}

void NetworkHandler::sendData(char* data, int dataLength)
{
	boost::asio::write(socket, boost::asio::buffer(data, dataLength));
}

std::vector<char> NetworkHandler::receiveData()
{
	std::vector<char> ret = std::vector<char>();
	for (;;)
	{
		boost::array<char, 128> buf;
		boost::system::error_code error;


		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		for (int i = 0; i < len; i++)
		{
			ret.push_back(buf[i]);
		}

		if (error == boost::asio::error::eof || error.value() == 0)
			break; // Connection closed cleanly by peer.
		else if (error)
			std::cout << "Networking error: " << error.message();
		throw boost::system::system_error(error);

	}
	return ret;
}

