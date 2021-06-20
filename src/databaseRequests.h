/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes
#include "dto.h"
#include "projectMetadata.h"

// Crawler includes
#include "RunCrawler.h"

// Parser includes
#include "HashData.h"

// Spider includes
#include "CodeBlock.h"

// External includes
#include <string>
#include <vector>

#define DATABASE_UPLOAD_REQUEST "upld"
#define DATABASE_CHECK_UPLOAD_REQUEST "chup"
#define DATABASE_CHECK_REQUEST "chck"
#define DATABASE_GET_AUTHORS_BY_ID "idau"
#define DATABASE_GET_PROJECT_DATA "extp"
#define DATABASE_ADD_JOB "upjb"
#define DATABASE_CRAWL_DATA "upcd"
#define DATABASE_GET_NEXT_JOB "gtjb"
#define DATABASE_GET_MOST_RECENT_VERSION_PROJECT "gppr"

class NetworkHandler;

class DatabaseRequests
{
public:
	/// <summary>
	/// Sends a request to the database to upload the hashes given.
	/// </summary>
	/// <param name="hashes">The hashes to be uploaded.</param>
	/// <returns>The string that the database send back.</returns>
	static std::string uploadHashes(std::vector<HashData>& hashes,
		ProjectMetaData metaData,
		AuthorData& authorData,
		EnvironmentDTO* env,
		std::string prevCommitTime = "",
		std::vector<std::string> unchangedFiles = std::vector<std::string>());

	/// <summary>
	/// Sends a request to the database to find matching hashes in the database.
	/// </summary>
	/// <param name="hashes">The hashes you want to find in the database.</param>
	/// <returns>The matches that the database gives back.</returns>
	static std::string findMatches(std::vector<HashData> &hashes,
		EnvironmentDTO *env);

	/// <summary>
	/// Sends a request to the database to upload and check the given hashes.
	/// </summary>
	/// <param name="hashes">The hashes to be uploaded.</param>
	/// <returns>The string that the database send back.</returns>
	static std::string checkUploadHashes(std::vector<HashData> &hashes,
		ProjectMetaData metaData, 
		AuthorData &authorData,
		EnvironmentDTO* env,
		std::string prevCommitTime = "",
		std::vector<std::string> unchangedFiles = std::vector<std::string>());

	/// <summary>
	/// Sends a request to the database get the author name and mail from the given author ids.
	/// </summary>
	/// <param name="authors">The authors you want the data from.</param>
	/// <returns>The string that the database send back.</returns>
	static std::string getAuthor(
		const std::map<std::string, int> &authors,
		EnvironmentDTO *env);

	/// <summary>
	/// Sends a request to the database to upload and check the given hashes.
	/// </summary>
	/// <returns>The string that the database send back.</returns>
	static std::string getProjectData(
		const std::map<
			std::pair<std::string, std::string>, 
			int> &projects,
		EnvironmentDTO *env);

	/// <summary>
	/// Sends a request to the database to get information of the newest version of a project.
	/// </summary>
	/// <returns>The string that the database send back.</returns>
	static long long getProjectVersion(const std::pair<std::string, std::string>& project,
		EnvironmentDTO* env);

	/// <summary>
	/// Sends a get next job request to the api.
	/// </summary>
	/// <returns>The next job the worked node should do.</returns>
	static std::string getNextJob(EnvironmentDTO *env);

	/// <summary>
	/// Sends a request to the api to add the given jobs to the job queue.
	/// </summary>
	static std::string addJobs(
		const std::vector<std::string>& jobs,
		EnvironmentDTO *env);

	/// <summary>
	/// Adds the jobs the crawler found to the job queue.
	/// </summary>
	/// <param name="jobs">The jobs to be added.</param>
	static std::string addCrawledJobs(
		const CrawlData& jobs,
		EnvironmentDTO *env);
private:
	/// <summary>
	/// The logic for sending a request to the database.
	/// A request in this case will contain 3 parts:
	///		1: sending to the API what request we want to do + how much data we are sending.
	///		2: sending the data for the request.
	///		3: Receiving the data the API returns.
	/// </summary>
	/// <param name="request">The request we are sending to the API</param>
	/// <param name="rawData">THIS DATA WILL BE AUTOMATICALLY DELETED AFTER IT HAS BEEN SENT BY THIS FUNCTION.
	/// The data we want to send to the API. 
	/// This data can most likely be generated with a function in NetworkUtils like getAllDataFromHashes.</param>
	/// <param name="dataSize">How much data we are sending.</param>
	/// <returns>The string that the database send back.</returns>
	static std::string execRequest(std::string request, 
		const char* rawData, 
		int dataSize, 
		EnvironmentDTO *env);

	/// <summary>
	/// Checks if the database returns a valid response code and handles potential errors.
	/// </summary>
	/// <returns> The data it receives minus the status code it checked. </returns>
	static std::tuple<bool, std::string> checkResponseCode(std::string data, std::string command);

	/// <summary>
	/// Will open a connection with the database API. 
	/// You will still need to manually delete the NetworkHandler* to close the connection.
	/// </summary>
	static NetworkHandler* startConnection(std::string apiIP, std::string apiPort);
};
