/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#include "DatabaseRequests.h"
#include "NetworkUtils.h"
#include "Networking.h"
#include "parser/Parser/Parser.h"

#define DATABASE_API_IP "131.211.31.153"
#define DATABASE_API_PORT "8003"

#define DATABASE_UPLOAD_REQUEST "upld"
#define DATABASE_CHECK_UPLOAD_REQUEST "chup"
#define DATABASE_CHECK_REQUEST "chck"

std::string DatabaseRequests::uploadHashes(std::vector<HashData> hashes, ProjectMetaData metaData)
{
    int dataSize = 0;
    //std::string header = NetworkUtils::generateHeader({ "3db1a3fc-24d8-4017-8d8b-7a2673625c58", "3", "license", "projectName", "url", "authorName", "authorMail", "5" });
    char* rawData = NetworkUtils::getAllDataFromHashes(hashes, dataSize, metaData.getAsHeader());

    return execRequest(DATABASE_UPLOAD_REQUEST, rawData, dataSize);
}

std::string DatabaseRequests::findMatches(std::vector<HashData> hashes)
{
    int dataSize = 0;
    char* rawData = NetworkUtils::getHashDataFromHashes(hashes, dataSize);

    return execRequest(DATABASE_CHECK_REQUEST, rawData, dataSize);
}

std::string DatabaseRequests::checkUploadHashes(std::vector<HashData> hashes, ProjectMetaData metaData)
{
    int dataSize = 0;
    char* rawData = NetworkUtils::getAllDataFromHashes(hashes, dataSize, metaData.getAsHeader());

    return execRequest(DATABASE_CHECK_UPLOAD_REQUEST, rawData, dataSize);
}

std::string DatabaseRequests::execRequest(std::string request, char* rawData, int dataSize)
{
    // First start the connection
    NetworkHandler* networkHandler = startConnection();

    // Then send the header (what request we are doing and how much data we are sending)
    std::string requestType = request + std::to_string(dataSize) + "\n";
    networkHandler->sendData(requestType);

    // After that we send the data
    networkHandler->sendData(rawData, dataSize);

    // Then we wait for the output that the API gives us
    std::string output = networkHandler->receiveData();

    // Deleting the data we send and closing the connection
    delete[] rawData;
    delete networkHandler;

    return output;
}

NetworkHandler* DatabaseRequests::startConnection()
{
    NetworkHandler* nh = NetworkHandler::createHandler();
    nh->openConnection(DATABASE_API_IP, DATABASE_API_PORT);
    return nh;
}
