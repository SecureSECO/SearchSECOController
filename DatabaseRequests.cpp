#include "DatabaseRequests.h"
#include "NetworkUtils.h"

#define DATABASE_API_IP "TODO: database api ip here"
#define DATABASE_API_PORT "Also TODO"

#define DATABASE_UPLOAD_REQUEST "addp"
#define DATABASE_FIND_REQUEST "find"

std::string DatabaseRequests::uploadHashes(std::vector<HashData> hashes)
{
    int dataSize = 0;
    std::string header = NetworkUtils::generateHeader({ "1", "3", "license", "projectName", "url", "authorName", "authorMail", "5" });
    char* rawData = NetworkUtils::getAllDataFromHashes(hashes, dataSize, header);

    return execRequest(DATABASE_UPLOAD_REQUEST, rawData, dataSize);
}

std::string DatabaseRequests::findMatches(std::vector<HashData> hashes)
{
    int dataSize = 0;
    char* rawData = NetworkUtils::getHashDataFromHashes(hashes, dataSize);

    return execRequest(DATABASE_FIND_REQUEST, rawData, dataSize);
}

std::string DatabaseRequests::execRequest(std::string request, char* rawData, int dataSize)
{
    NetworkHandler* networkHandler = startConnection();
    std::string requestType = request + std::to_string(dataSize);
    networkHandler->sendData(requestType);

    networkHandler->sendData(rawData, dataSize);

    std::string output = networkHandler->receiveData();

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
