/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
? Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// TODO ILAN: schrijf test
#include "pch.h"
#include "DatabaseAPIMock.h"

#include "DatabaseRequests.h"

#include <thread>

TEST(IntegrationDatabaseAPITest, BasicTest)
{
    ConnectionHandler* connectionHandler = new ConnectionHandler();
    std::thread* t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);
    // requestHandler.StartListen();
    
    std::vector<HashData> hashes = {HashData("hash", "functionname", "filename", 5, 420) };
    //Sleep(1000);
    std::string result = DatabaseRequests::uploadHashes(
        hashes, ProjectMetaData("5", "2", "Lyzenze", "ProjectName", "url.com", "authorName", "author@mail.com"), {},
        "127.0.0.1", "6969");
    EXPECT_EQ(result, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");


}