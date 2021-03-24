#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

struct HashData;

class NetworkHandler
{
public:

	NetworkHandler();

	/// <summary>
	/// Send data to the database.
	/// </summary>
	/// <param name="data">The data to be send</param>
	/// <param name="dataSize">The size of the data array in bytes</param>
	void sendData(char* data, int dataSize);
	void sendData(std::string data) { sendData( &(data[0]), data.length()); };


	std::vector<char> receiveData();
private:
	/// <summary>
	/// Opens a connection to the database.
	/// </summary>
	void openConnection();

	tcp::socket socket;
	boost::asio::io_context io_context;
};

