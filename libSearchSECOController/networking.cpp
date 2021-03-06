/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "networking.h"
#include "print.h"
#include "utils.h"
#include "databaseRequests.h"

// External includes.
#include <boost/array.hpp>
#include <boost/optional.hpp>
#include <fstream>
#include <filesystem>


std::vector<std::pair<std::string, std::string>> NetworkHandler::ips = {};
bool refreshIPs = true;

extern std::atomic<bool> stopped;

// https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio/tutorial.html was used as a base.
boost::asio::io_context NetworkHandler::ioContext;

void NetworkHandler::openConnection(EnvironmentDTO *env)
{
	print::debug("Opening a new connection", __FILE__, __LINE__);

	// If both server and port are both -1, we will read the .env file to find the servers we want to connect to.
	if (env->databaseAPIIP == "-1" && env->databaseAPIPort == "-1")
	{
		if (ips.size() == 0)
		{
			readEnvFile();
		}
		if (refreshIPs)
		{
			refreshIPs = false;
			std::string newIPs = DatabaseRequests::getIPs(env);
			ips = parseIPs(newIPs);
			refreshIPs = false;
		}

		auto ipList = ips;
		// Shuffle so we always connect to a random one first.
		Utils::shuffle(ipList);
		for (auto server : ipList)
		{
			try
			{
				print::debug("Connecting with " + server.first, __FILE__, __LINE__);
				connect(server.first, server.second);
				return;
			}
			catch (std::exception const& ex)
			{
				refreshIPs = true;
				print::warn("Could not establish a connection with " + server.first, __FILE__, __LINE__);
			}
		}
		error::errDBConnection("No connection could be established.", __FILE__, __LINE__);
	}
	else
	{
		try
		{
			connect(env->databaseAPIIP, env->databaseAPIPort);
		}
		catch (std::exception const& ex)
		{
			error::errDBConnection("Could not astablish a connection with " + env->databaseAPIIP, __FILE__, __LINE__);
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
	print::debug("Sending " + std::to_string(dataLength) + " bytes to database.", __FILE__, __LINE__);
	print::debug("Sending: " + std::string(data, dataLength), __FILE__, __LINE__);
	boost::asio::write(socket, boost::asio::buffer(data, dataLength));
}

std::string NetworkHandler::receiveData()
{
	print::debug("Listening for a database response", __FILE__, __LINE__);
	// The buffer we are going to return as a string.
	std::vector<char> ret = std::vector<char>();
	bool done = false;
	while (!done && !stopped)
	{
		boost::array<char, 128> buf;
		//boost::system::error_code error;

		// Read incoming data.
		//size_t len = socket.read_some(boost::asio::buffer(buf), error);

		boost::optional<boost::system::error_code> read_result;
		boost::asio::async_read(
			socket, boost::asio::buffer(buf),
			[&read_result, &ret, &done, &buf](const boost::system::error_code &error, size_t len) { read_result.reset(error); 

		print::debug("Receiving " + std::to_string(len) + " bytes", __FILE__, __LINE__);

		// Add it to out buffer.
		for (int i = 0; i < len; i++)
		{
			ret.push_back(buf[i]);
		}

		if (error == boost::asio::error::eof)
		{
			// Connection closed cleanly by peer.
			print::debug("Received data from database: " + std::string(ret.begin(), ret.end()), __FILE__, __LINE__);
			done = true;
		}
		else if (error)
		{
			// If any error occures, we just throw.
			error::errDBConnection(error.message(), __FILE__, __LINE__);
		}

		});

		ioContext.reset();
		while (ioContext.run_one())
		{
			if (stopped)
			{
				print::debug("Database request timed out. Closing socket.", __FILE__, __LINE__);
				socket.cancel();
			}
			else if (read_result)
				break;
		}

	}
	return std::string(ret.begin(), ret.end());
}

void NetworkHandler::readEnvFile()
{
	// Opening file.
	auto path = (std::filesystem::path(Utils::getExecutablePath()) / ".env")
		.string();

	std::ifstream fileHandler;
	fileHandler.open(path);

	if (!fileHandler.is_open())
	{
		error::errNoEnvFile(__FILE__, __LINE__);
		return;
	}

	// Read line by line.
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
			// If the ips is not empty, then that means we have read the API_IPS line and can return.
			if (ips.size() > 0)
			{
				return;
			}
		}
	}
	error::errNoIpsInEnvFile(__FILE__, __LINE__);
}

std::vector<std::pair<std::string, std::string>> NetworkHandler::parseIPs(std::string newIPs)
{
	std::vector<std::string> splitted = Utils::split(newIPs, '\n');
	std::vector<std::pair<std::string, std::string>> result = std::vector<std::pair<std::string, std::string>>();
	for (std::string ip : splitted)
	{
		print::log(ip, __FILE__, __LINE__);
		if (ip.size() <= 1)
		{
			continue;
		}
		std::vector<std::string> splittedIP = Utils::split(ip, '?');
		if (splittedIP.size() < 2)
		{
			continue;
		}
		result.push_back(std::pair<std::string, std::string>(splittedIP[0], splittedIP[1]));
	}
	return result;
}
