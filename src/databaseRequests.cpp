/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "databaseRequests.h"
#include "networkUtils.h"
#include "networking.h"

// Parser includes
#include "Parser.h"


std::string DatabaseRequests::uploadHashes(std::vector<HashData> hashes, ProjectMetaData metaData, AuthorData authorData, std::string apiIP, std::string apiPort)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAllDataFromHashes(hashes, dataSize, metaData.getAsHeader(), authorData);
	return execRequest(DATABASE_UPLOAD_REQUEST, rawData, dataSize, apiIP, apiPort);
}

std::string DatabaseRequests::findMatches(std::vector<HashData> hashes, std::string apiIP, std::string apiPort)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getHashDataFromHashes(hashes, dataSize);

	return execRequest(DATABASE_CHECK_REQUEST, rawData, dataSize, apiIP, apiPort);
}

std::string DatabaseRequests::checkUploadHashes(std::vector<HashData> hashes, ProjectMetaData metaData, AuthorData authorData, std::string apiIP, std::string apiPort)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAllDataFromHashes(hashes, dataSize, metaData.getAsHeader(), authorData);

	return execRequest(DATABASE_CHECK_UPLOAD_REQUEST, rawData, dataSize, apiIP, apiPort);
}

std::string DatabaseRequests::getAuthor(std::map<std::string, int> authors, std::string apiIP, std::string apiPort)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getAuthorStringToSend(authors, dataSize);

	return execRequest(DATABASE_GET_AUTHORS_BY_ID, rawData, dataSize, apiIP, apiPort);
}

std::string DatabaseRequests::getProjectData(std::map<std::pair<std::string, std::string>, int> authors,
	std::string apiIP, std::string apiPort)
{
	int dataSize = 0;
	const char* rawData = NetworkUtils::getProjectsRequest(authors, dataSize);

	return execRequest(DATABASE_GET_PROJECT_DATA, rawData, dataSize, apiIP, apiPort);
}

std::string DatabaseRequests::execRequest(std::string request, const char* rawData, int dataSize, std::string apiIP, std::string apiPort)
{
	// First start the connection.
	NetworkHandler* networkHandler = startConnection(apiIP, apiPort);

	// Then send the header (what request we are doing and how much data we are sending).
	std::string requestType = request + std::to_string(dataSize) + "\n";
	networkHandler->sendData(requestType);

	// After that we send the data.
	networkHandler->sendData(rawData, dataSize);

	// Then we wait for the output that the API gives us.
	std::string output = networkHandler->receiveData();

	// Deleting the data we send and closing the connection.
	delete[] rawData;
	delete networkHandler;

	return output;
}

NetworkHandler* DatabaseRequests::startConnection(std::string apiIP, std::string apiPort)
{
	NetworkHandler* nh = NetworkHandler::createHandler();
	nh->openConnection(apiIP, apiPort);
	return nh;
}
