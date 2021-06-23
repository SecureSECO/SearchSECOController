/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "flags.h"

// Crawler includes.
#include "RunCrawler.h"
#include "ProjectMetadata.h"


namespace moduleFacades
{
	/// <summary>
	/// Will call the spider to download a given repository.
	/// </summary>
	std::tuple<AuthorData, std::string, std::vector<std::string>> downloadRepository(
		std::string repository, 
		Flags flags,
		std::string downloadPath, 
		std::string tag = "HEAD", 
		std::string nextTag = "");
	
	/// <summary>
	/// Will call the spider to download a given repository.
	/// </summary>
	std::vector<std::pair<std::string, long long>> getRepositoryTags(std::string downloadPath);

	
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
