#include "Networking.h"
#include <boost/array.hpp>

// Made using this tutorial https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio/tutorial.html
tcp::socket Networking::openConnection()
{
	std::string serverApi = "TODO: server name and shit";

	boost::asio::io_context io_context;
	tcp::resolver resolver(io_context);
	tcp::resolver::results_type endpoints = resolver.resolve(serverApi, "TODO: something here");

	tcp::socket socket(io_context);
	boost::asio::connect(socket, endpoints);
	return socket;
}

void Networking::sendData(byte* data, int dataLength, tcp::socket socket)
{
	boost::asio::write(socket, boost::asio::buffer(data, dataLength));
}

std::vector<char> Networking::receiveData(tcp::socket socket)
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
