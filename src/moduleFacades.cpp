/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "moduleFacades.h"
#include "print.h"
#include "termination.h"

// Crawler includes
#include "RunCrawler.h"

// Parser includes.
#include "Parser.h"

// Spider includes.
#include "RunSpider.h"

// Crawler includes.
#include "RunCrawler.h"

AuthorData moduleFacades::downloadRepository(std::string repository, Flags flags, std::string downloadPath)
{
	print::debug("Calling the spider to download a repository", __FILE__, __LINE__);

	auto authorData = RunSpider::runSpider(repository, downloadPath, flags.flag_cpu, flags.flag_branch);

	return authorData;
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

ProjectMetaData moduleFacades::getProjectMetadata(std::string url)
{
	print::debug("Calling the crawler to get the metadata from a project", __FILE__, __LINE__);

	ProjectMetadata pmd = RunCrawler::findMetadata(url);	

	int er = errno;
	// TODO: very temporary hashing.
	std::string id = pmd.authorMail + pmd.authorName + pmd.version;
	long long hash = 0;
	for (int i = 0; i < id.size(); i++)
	{
		hash += id[i] * (i + 1);
	}
	ProjectMetaData pm = ProjectMetaData(std::to_string(hash),
		std::to_string(utils::getIntegerTimeFromString(pmd.version)),
		pmd.license,
		pmd.name,
		pmd.url,
		pmd.authorName,
		pmd.authorMail,
		pmd.defaultBranch);
	errno = er;
	return pm;
}

CrawlData moduleFacades::crawlRepositories(int startId)
{
	print::debug("Calling the crawler to crawl a repositories", __FILE__, __LINE__);
	auto urls = RunCrawler::crawlRepositories("https://github.com/", startId);
	return urls;
}
