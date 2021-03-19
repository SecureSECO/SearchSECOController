#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

typedef unsigned char byte;

class Networking
{
public:
	static tcp::socket openConnection();

	/// <summary>
	/// Send data to the database
	/// </summary>
	/// <param name="data">The data to be send</param>
	/// <param name="dataSize">The size of the data array in bytes</param>
	static void sendData(byte* data, int dataSize, tcp::socket socket);

	static std::vector<char> receiveData(tcp::socket socket);
};

