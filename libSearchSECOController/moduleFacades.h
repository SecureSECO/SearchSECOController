/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "flags.h"
#include "projectMetadata.h"

// Spider includes.
#include "CodeBlock.h"
#include "Spider.h"

// Crawler includes.
#include "RunCrawler.h"

// Parser includes.
#include "HashData.h"


namespace moduleFacades
{
	/// <summary>
	/// Sets up specific spider, based on host website of the project and max number of threads.
	/// </summary>
	/// <param name="repository"> Link to repository to download. </param>
	/// <param name="flags"> Flags to use. </param>
	/// <returns> Pointer to Spider. </returns>
	Spider* setupSpider(std::string repository, Flags flags);

	/// <summary>
	/// Downloads a repository from a given source and stores it
	/// locally at the location defined by filePath.
	/// </summary>
	/// <param name="s"> Specific spider to use. </param>
	/// <param name="repository"> Url to source to download. </param>
	/// <param name="flags"> Flags to use. </param>
	/// <param name="downloadPath"> Local path where to store the source. </param>
	void downloadRepo(Spider *s, std::string repository, Flags flags, std::string downloadPath);
	
	/// <summary>
	/// Updates project from one version to another, keeping track of unchanged files.
	/// Versions should be in chronological order. Deletes unchanged files from local project.
	/// </summary>
	/// <param name="s"> Specific spider to use. </param>
	/// <param name="repository"> Local path where project is stored. </param>
	/// <param name="prevTag"> Name of current version. </param>
	/// <param name="newTag"> Name of version to update to. </param>
	/// <param name="prevUnchangedFiles"> Name of previous unchanged files, which were
	/// deleted from the local project. </param>
	/// <returns> Unchanged files between versions. </returns>
	std::vector<std::string> updateVersion(Spider *s, std::string repository, std::string prevTag,
										   std::string newTag, std::vector<std::string> prevUnchangedFiles);

	/// <summary>
	/// Switches local project to different version.
	/// </summary>
	/// <param name="s"> Specific spider to use. </param>
	/// <param name="tag"> Name of the version to update to. </param>
	/// <param name="repository"> Local path where project is stored. </param>
	void switchVersion(Spider *s, std::string tag, std::string repository);

	void trimFiles(Spider *s, std::map<std::string, std::vector<int>> lines, std::string repository);

	/// <summary>
	/// Extracts author data from locally stored project.
	/// </summary>
	/// <param name="s"> Specific spider to use. </param>
	/// <param name="repository"> Local path where project is stored. </param>
	/// <returns> AuthorData object containing extracted author data. </returns>
	AuthorData getAuthors(Spider *s, std::string repository);

	/// <summary>
	/// Returns the current version of the locally stored project.
	/// </summary>
	/// <param name="s"> The Spider to use. </param>
	/// <param name="repository"> Local path where project is stored. </param>
	/// <returns> String containing current version. </returns>
	std::string currentVersion(Spider *s, std::string repository);

	/// <summary>
	/// Will call the spider to download a given repository.
	/// </summary>
	std::vector<std::tuple<std::string, long long, std::string>> getRepositoryTags(std::string downloadPath);

	
	/// <summary>
	/// Will call the parser to parse the given repository.
	/// </summary>
	std::vector<HashData> parseRepository(std::string repository, Flags flags);

	/// <summary>
	/// Calls the crawler to get the metadata from the given url.
	/// </summary>
	ProjectMetaData getProjectMetadata(std::string url, Flags flags);

	/// <summary>
	/// Runs the crawler and gets the next 100 repositories to add to the job queue.
	/// </summary>
	CrawlData crawlRepositories(int startId, Flags flags);
};
