/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "databaseAPIMock.h"
#include "databaseRequests.h"

// External includes
#include <thread>


// Constants.
#define LOCALHOST "127.0.0.1"
#define PORT "6969"

// Dummies
#define DUMMYHASHES { HashData("hash", "functionname", "filename", 5, 420) }


TEST(integrationDatabaseAPI, upload)
{
    std::vector<HashData> dummyHashes = DUMMYHASHES;
    ConnectionHandler *connectionHandler = new ConnectionHandler();
    std::thread *t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);

    AuthorData ad = {};
    std::string result = DatabaseRequests::uploadHashes(
        dummyHashes,
        ProjectMetaData("5", "2", "Lyzenze", "ProjectName", "url.com", "authorName", "author@mail.com"), 
        ad,
        LOCALHOST,
        PORT
    );
    EXPECT_EQ(result, "Request received from upld");
}

TEST(integrationDatabaseAPI, check)
{
    std::vector<HashData> dummyHashes = DUMMYHASHES;
    ConnectionHandler *connectionHandler = new ConnectionHandler();
    std::thread *t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);

    std::string result = DatabaseRequests::findMatches(
        dummyHashes,
        LOCALHOST, 
        PORT
    );
    EXPECT_EQ(result, "Request received from chck");
}

TEST(integrationDatabaseAPI, checkUpload)
{
    std::vector<HashData> dummyHashes = DUMMYHASHES;
    ConnectionHandler* connectionHandler = new ConnectionHandler();
    std::thread* t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);
    
    AuthorData ad = {};
    std::string result = DatabaseRequests::checkUploadHashes(
        dummyHashes,
        ProjectMetaData("5", "2", "Lyzenze", "ProjectName", "url.com", "authorName", "author@mail.com"), 
        ad,
        LOCALHOST, 
        PORT
    );
    EXPECT_EQ(result, "Request received from chup");
}

TEST(integrationDatabaseAPI, wrongPortCheck)
{
    std::vector<HashData> dummyHashes = DUMMYHASHES;
    ConnectionHandler* connectionHandler = new ConnectionHandler();
    std::thread* t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);

    EXPECT_THROW(DatabaseRequests::findMatches(dummyHashes, LOCALHOST, "wrongport"), std::runtime_error);
}
