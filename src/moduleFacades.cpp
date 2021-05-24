/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "moduleFacades.h"
#include "print.h"

// Parser includes.
#include "Parser.h"

// Spider includes.
#include "RunSpider.h"

// Crawler includes.
#include "RunCrawler.h"

AuthorData moduleFacades::downloadRepository(std::string repository, Flags flags, std::string downloadPath)
{
	print::debug("Calling the spider to download a repository", __FILE__, __LINE__);

	return RunSpider::runSpider(repository, downloadPath, flags.flag_cpu, flags.flag_branch);
}

std::vector<HashData> moduleFacades::parseRepository(std::string repository, Flags flags)
{
	print::debug("Calling the parser to parse a repository", __FILE__, __LINE__);

	auto hashes = Parser::parse(repository, flags.flag_cpu);
	for (int i = 0; i < hashes.size(); i++)
	{
		utils::replace(hashes[i].fileName, '/', '\\');
	}
	return hashes;
}

CrawlData moduleFacades::crawlRepositories(int startId)
{
	print::debug("Calling the crawler to crawl a repositories", __FILE__, __LINE__);
	int error = 0;
	auto urls = RunCrawler::crawlRepositories("https://github.com/", startId, error);
	if (error != 0)
	{
		print::warn("Crawler returned with error code " + std::to_string(error), __FILE__, __LINE__);
	}
	return urls;
}
