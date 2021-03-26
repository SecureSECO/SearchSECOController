#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

struct HashData;

class NetworkHandler
{
public:
	static NetworkHandler* createHandler();
	/// <summary>
	/// Send data to the database.
	/// </summary>
	/// <param name="data">The data to be send</param>
	/// <param name="dataSize">The size of the data array in bytes</param>
	void sendData(char* data, int dataSize);
	void sendData(std::string data) { sendData(&(data[0]), data.length()); };

	/// <summary>
	/// Opens a connection to the database.
	/// </summary>
	void openConnection(std::string server, std::string port);

	std::string receiveData();
private:
	static boost::asio::io_context io_context;
	NetworkHandler(boost::asio::io_context& io_context)
		: socket(io_context)
	{
	};

	tcp::socket socket;
};
