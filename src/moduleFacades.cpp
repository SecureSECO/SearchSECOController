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
#include "md5/md5.h"
#include "Parser.h"

// Spider includes.
#include "RunSpider.h"
#include <iostream>


std::tuple<AuthorData, std::string, std::vector<std::string>> moduleFacades::downloadRepository(std::string repository, Flags flags,
	std::string downloadPath, std::string tag, std::string nextTag)
{
	print::debug("Calling the spider to download a repository", __FILE__, __LINE__);

	std::tuple<AuthorData, std::string, std::vector<std::string>> authorData = RunSpider::runSpider(repository, downloadPath, flags.flag_cpu, tag, nextTag, flags.flag_branch);
	print::loguruResetThreadName();

	return authorData;
}

std::vector<std::pair<std::string, long long>> moduleFacades::getRepositoryTags(std::string downloadPath)
{
	print::debug("Calling the spider to get tags of previous versions", __FILE__, __LINE__);

	std::vector<std::pair<std::string,long long>> tags = RunSpider::getTags(downloadPath);

	return tags;
}

std::vector<HashData> moduleFacades::parseRepository(std::string repository, Flags flags)
{
	print::debug("Calling the parser to parse a repository", __FILE__, __LINE__);

	auto hashes = Parser::parse(repository, flags.flag_cpu);
	print::loguruResetThreadName();

	for (int i = 0; i < hashes.size(); i++)
	{
		utils::replace(hashes[i].fileName, '/', '\\');
	}
	return hashes;
}

ProjectMetaData moduleFacades::getProjectMetadata(std::string url, Flags flags)
{
	print::debug("Calling the crawler to get the metadata from a project", __FILE__, __LINE__);

	ProjectMetadata pmd = RunCrawler::findMetadata(url, flags.flag_github_user, flags.flag_github_token);
	int er = errno;
	print::loguruResetThreadName();

	std::string versionHash = ""; // TODO get commit hash from spider

	// TODO: very temporary hashing.
	long long hash = utils::getIdFromPMD(pmd);


	ProjectMetaData pm = ProjectMetaData(std::to_string(hash),
		std::to_string(utils::getIntegerTimeFromString(pmd.version)),
		versionHash,
		pmd.license,
		pmd.name,
		pmd.url,
		pmd.authorName,
		pmd.authorMail,
		pmd.defaultBranch);
	errno = er;
	return pm;
}

CrawlData moduleFacades::crawlRepositories(int startId, Flags flags)
{
	print::debug("Calling the crawler to crawl a repositories", __FILE__, __LINE__);

	auto urls = RunCrawler::crawlRepositories("https://github.com/", startId, flags.flag_github_user, flags.flag_github_token);
	print::loguruResetThreadName();

	return urls;
}
