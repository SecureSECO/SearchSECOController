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
    ConnectionHandler *connectionHandler = new ConnectionHandler();
    std::thread *t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);

    std::string result = DatabaseRequests::uploadHashes(
        DUMMYHASHES, 
        ProjectMetaData("5", "2", "Lyzenze", "ProjectName", "url.com", "authorName", "author@mail.com"), 
        {},
        LOCALHOST,
        PORT
    );
    EXPECT_EQ(result, "Request received from upld");
}

TEST(integrationDatabaseAPI, check)
{
    ConnectionHandler *connectionHandler = new ConnectionHandler();
    std::thread *t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);

    std::string result = DatabaseRequests::findMatches(
        DUMMYHASHES, 
        LOCALHOST, 
        PORT
    );
    EXPECT_EQ(result, "Request received from chck");
}

TEST(integrationDatabaseAPI, checkUpload)
{
    ConnectionHandler* connectionHandler = new ConnectionHandler();
    std::thread* t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);
    
    std::string result = DatabaseRequests::checkUploadHashes(
        DUMMYHASHES, 
        ProjectMetaData("5", "2", "Lyzenze", "ProjectName", "url.com", "authorName", "author@mail.com"), 
        {},
        LOCALHOST, 
        PORT
    );
    EXPECT_EQ(result, "Request received from chup");
}

TEST(integrationDatabaseAPI, wrongPortCheck)
{
    ConnectionHandler* connectionHandler = new ConnectionHandler();
    std::thread* t1 = new std::thread(&ConnectionHandler::StartListen, connectionHandler);

    EXPECT_THROW(DatabaseRequests::findMatches(DUMMYHASHES, LOCALHOST, "wrongport"), std::runtime_error);
}