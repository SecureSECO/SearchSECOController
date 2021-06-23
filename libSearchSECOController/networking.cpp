/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "networking.h"
#include "print.h"

// External includes.
#include <boost/array.hpp>
#include <fstream>
#include <filesystem>
#include <iostream>


std::vector<std::pair<std::string, std::string>> NetworkHandler::ips = {};

// https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio/tutorial.html was used as a base.
boost::asio::io_context NetworkHandler::ioContext;

void NetworkHandler::openConnection(std::string server, std::string port)
{
	print::debug("Opening a new connection", __FILE__, __LINE__);

	if (server == "-1" && port == "-1")
	{
		if (ips.size() == 0)
		{
			readEnvFile();
		}
		auto ipList = ips;
		Utils::shuffle(ipList);
		for (auto server : ipList)
		{
			try
			{
				print::log("Connecting with " + server.first, __FILE__, __LINE__);
				connect(server.first, server.second);
				return;
			}
			catch (std::exception const& ex)
			{
				print::warn("Could not establish a connection with " + server.first, __FILE__, __LINE__);
			}
		}
		error::errDBConnection("No connection could be established.", __FILE__, __LINE__);
	}
	else
	{
		try
		{
			connect(server, port);
		}
		catch (std::exception const& ex)
		{
			error::errDBConnection("Could not astablish a connection with " + server, __FILE__, __LINE__);
		}
	}
}

void NetworkHandler::connect(std::string server, std::string port)
{
	tcp::resolver resolver(ioContext);
	tcp::resolver::results_type endpoints = resolver.resolve(server, port);

	boost::asio::connect(socket, endpoints);
}

NetworkHandler* NetworkHandler::createHandler()
{
	return new NetworkHandler(ioContext);
}

void NetworkHandler::sendData(const char* data, int dataLength)
{
	print::log("Sending " + std::to_string(dataLength) + " bytes to database.", __FILE__, __LINE__);
	boost::asio::write(socket, boost::asio::buffer(data, dataLength));
}

std::string NetworkHandler::receiveData()
{
	print::debug("Listening for a database response", __FILE__, __LINE__);
	// The buffer we are going to return as a string.
	std::vector<char> ret = std::vector<char>();
	for (;;)
	{
		boost::array<char, 128> buf;
		boost::system::error_code error;

		// Read incoming data.
		size_t len = socket.read_some(boost::asio::buffer(buf), error);
		print::debug("Receiving " + std::to_string(len) + " bytes", __FILE__, __LINE__);

		// Add it to out buffer.
		for (int i = 0; i < len; i++)
		{
			ret.push_back(buf[i]);
		}

		if (error == boost::asio::error::eof)
		{
			// Connection closed cleanly by peer.
			print::log("Received data from database.", __FILE__, __LINE__);
			break;
		}
		else if (error)
		{
			// If any error occures, we just throw.
			error::errDBConnection(error.message(), __FILE__, __LINE__);
		}

	}
	return std::string(ret.begin(), ret.end());
}

void NetworkHandler::readEnvFile()
{
	auto path = (std::filesystem::path(Utils::getExecutablePath()) / ".env")
		.string();

	std::ifstream fileHandler;
	fileHandler.open(path);

	if (!fileHandler.is_open())
	{
		error::errNoEnvFile(__FILE__, __LINE__);
		return;
	}
	std::string line;
	ips = std::vector<std::pair<std::string, std::string>>();
	while (std::getline(fileHandler, line))
	{
		auto lineSplitted = Utils::split(line, '=');
		if (lineSplitted.size() >= 2 && lineSplitted[0] == "API_IPS")
		{
			auto ipsSplitted = Utils::split(lineSplitted[1], ',');
			for (std::string ip : ipsSplitted)
			{
				auto ipSplitted = Utils::split(ip, '?');
				if (ipSplitted.size() < 2)
				{
					continue;
				}
				ips.push_back(std::pair<std::string, std::string>(ipSplitted[0], ipSplitted[1]));
			}
			if (ips.size() > 0)
			{
				return;
			}
		}
	}
	error::errNoIpsInEnvFile(__FILE__, __LINE__);
}
