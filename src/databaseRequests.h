/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes
#include "projectMetadata.h"

// Parser includes
#include "HashData.h"

// Spider includes
#include "CodeBlock.h"

// External includes
#include <string>
#include <vector>


#define DATABASE_API_IP "131.211.31.153"
#define DATABASE_API_PORT "8003"

#define DATABASE_UPLOAD_REQUEST "upld"
#define DATABASE_CHECK_UPLOAD_REQUEST "chup"
#define DATABASE_CHECK_REQUEST "chck"
#define DATABASE_GET_AUTHORS_BY_ID "idau"
#define DATABASE_GET_PROJECT_DATA "extp"

class NetworkHandler;

class DatabaseRequests
{
public:
	/// <summary>
	/// Sends a request to the database to upload the hashes given.
	/// </summary>
	/// <param name="hashes">The hashes to be uploaded.</param>
	/// <returns>The string that the database send back.</returns>
	static std::string uploadHashes(std::vector<HashData> &hashes, 
		ProjectMetaData metaData, 
		AuthorData &authorData, 
		std::string apiIP = DATABASE_API_IP, 
		std::string apiPort = DATABASE_API_PORT);

	/// <summary>
	/// Sends a request to the database to find matching hashes in the database.
	/// </summary>
	/// <param name="hashes">The hashes you want to find in the database.</param>
	/// <returns>The matches that the database gives back.</returns>
	static std::string findMatches(std::vector<HashData> &hashes,
		std::string apiIP = DATABASE_API_IP, 
		std::string apiPort = DATABASE_API_PORT);

	/// <summary>
	/// Sends a request to the database to upload and check the given hashes.
	/// </summary>
	/// <param name="hashes">The hashes to be uploaded.</param>
	/// <returns>The string that the database send back.</returns>
	static std::string checkUploadHashes(std::vector<HashData> &hashes,
		ProjectMetaData metaData, 
		AuthorData &authorData, 
		std::string apiIP = DATABASE_API_IP, 
		std::string apiPort = DATABASE_API_PORT);

	/// <summary>
	/// Sends a request to the database get the author name and mail from the given author ids.
	/// </summary>
	/// <param name="authors">The authors you want the data from.</param>
	/// <returns>The string that the database send back.</returns>
	static std::string getAuthor(const std::map<std::string, int> &authors,
		std::string apiIP = DATABASE_API_IP,
		std::string apiPort = DATABASE_API_PORT);

	/// <summary>
	/// Sends a request to the database to upload and check the given hashes.
	/// </summary>
	/// <returns>The string that the database send back.</returns>
	static std::string getProjectData(const std::map<std::pair<std::string, std::string>, int> &projects,
		std::string apiIP = DATABASE_API_IP,
		std::string apiPort = DATABASE_API_PORT);
private:
	/// <summary>
	/// The logic for sending a request to the database.
	/// A request in this case will contain 3 parts:
	///		1: sending to the API what request we want to do + how much data we are sending.
	///		2: sending the data for the request.
	///		3: Receiving the data the API returns.
	/// </summary>
	/// <param name="request">The request we are sending to the API</param>
	/// <param name="rawData">THIS DATA WILL BE AUTOMATICLY DELETED AFTER IT HAS BEEN SEND BY THIS FUNCTION.
	/// The data we want to send to the API. 
	/// This data can most likely be generated with a function in NetworkUtils like getAllDataFromHashes.</param>
	/// <param name="dataSize">How much data we are sending.</param>
	/// <returns>The string that the database send back.</returns>
	static std::string execRequest(std::string request, const char* rawData, int dataSize, std::string apiIP, std::string apiPort);

	/// <summary>
	/// Will open a connection with the database API. 
	/// You will still need to manually delete the NetworkHandler* to close the connection.
	/// </summary>
	static NetworkHandler* startConnection(std::string apiIP, std::string apiPort);
};
