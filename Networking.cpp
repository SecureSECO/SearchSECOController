#include "Networking.h"
#include <boost/array.hpp>
#include "parser/Parser/Parser.h"

// Made using this tutorial https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio/tutorial.html

NetworkHandler::NetworkHandler() : socket(io_context)
{
}

void NetworkHandler::openConnection()
{
	std::string serverApi = "TODO: server name and shit";

	tcp::resolver resolver(io_context);
	tcp::resolver::results_type endpoints = resolver.resolve(serverApi, "TODO: something here");

	boost::asio::connect(socket, endpoints);
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
		if (error == boost::asio::error::eof)
			break; // Connection closed cleanly by peer.
		else if (error)
			throw boost::system::system_error(error);

		for (char c : buf)
		{
			ret.push_back(c);
		}
	}
	return ret;
}

