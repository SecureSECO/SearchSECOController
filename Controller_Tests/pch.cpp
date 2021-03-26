/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"
#include "../spider/SearchSECOSpider/SearchSecoSpider.h"
#include "../DatabaseRequests.h"
#include "../parser/Parser/parser.h"

// We don't want to use this in the unit tests, so we just do a empty function so that we don't
// have to link the spider
void RunSpider::runSpider(std::string s)
{
};


// Also don't want to include the databaserequest stuff
std::string DatabaseRequests::uploadHashes(std::vector<HashData> hashes)
{
    return "";
}

std::string DatabaseRequests::findMatches(std::vector<HashData> hashes)
{
    return "";
}

std::string DatabaseRequests::execRequest(std::string request, char* rawData, int dataSize)
{
    return "";
}

NetworkHandler* DatabaseRequests::startConnection()
{
    
    return nullptr;
}