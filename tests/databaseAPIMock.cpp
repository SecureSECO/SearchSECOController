/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "databaseAPIMock.h"
#include "utils.h"
#include "networkUtils.h"


// Connection Handler Methods
void ConnectionHandler::StartListen()
{
	try
	{
		boost::asio::io_context io_context;
		tcp_server server(io_context);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

std::string RequestHandler::HandleRequest(std::string sender, std::string request)
{
	return "200\nRequest received from " + sender;
}	


// TCP Connection Methods 
tcp_connection::pointer tcp_connection::create(boost::asio::io_context& io_context)
{
	return pointer(new tcp_connection(io_context));
}

void tcp_connection::start(RequestHandler handler)
{

	std::vector<char> request = std::vector<char>();
	boost::system::error_code error;

	size_t len = boost::asio::read_until(socket_, boost::asio::dynamic_buffer(request), '\n');

	std::string r(request.begin(), request.begin() + len - 1);

	std::vector<std::string> header = Utils::split(r, INNER_DELIMITER);

	std::string length = header[2];
	std::string totalData(request.begin() + len, request.end());
	int size = std::stoi(length) - (request.size() - len);

	std::vector<char> data(size);

	while (size > 0)
	{
		int prevSize = size;
		size -= socket_.read_some(boost::asio::buffer(data), error);
		totalData.append(std::string(data.begin(), data.begin() + prevSize - size));
	}

	std::string result = handler.HandleRequest(header[0], totalData);
	std::cout << "sending " << result << "\n";
	boost::asio::write(socket_, boost::asio::buffer(result), error);


}

// TCP server Methods

tcp_server::tcp_server(boost::asio::io_context& io_context)
	: io_context_(io_context),
	acceptor_(io_context, tcp::endpoint(tcp::v4(), 6969))
{
	handler.Initialize();
	start_accept();
}

void tcp_server::start_accept()
{
	tcp_connection::pointer new_connection =
		tcp_connection::create(io_context_);

	acceptor_.async_accept(new_connection->socket(),
		boost::bind(&tcp_server::handle_accept, this, new_connection,
			boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection,
	const boost::system::error_code& error)
{
	start_accept();
	if (!error)
	{
		new_connection->start(handler);
	}

}
