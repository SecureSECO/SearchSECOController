/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "moduleFacades.h"
#include "print.h"
#include "utils.h"

// Spider includes.
#include "RunSpider.h"

#include <filesystem>


Spider* moduleFacades::setupSpider(std::string repository, Flags flags)
{
	return RunSpider::setupSpider(repository, flags.flag_cpu);
}

void moduleFacades::downloadRepo(Spider *s, std::string repository, Flags flags, std::string downloadPath)
{
	print::debug("Calling the spider to download a repository", __FILE__, __LINE__);

	RunSpider::downloadRepo(s, repository, downloadPath, flags.flag_branch);

	print::loguruResetThreadName();
}

std::vector<std::string> moduleFacades::updateVersion(Spider *s, std::string repository, std::string prevTag,
													  std::string newTag, std::vector<std::string> prevUnchangedFiles)
{
	print::debug("Calling the spider to switch from version " + prevTag + " to version " + newTag, __FILE__, __LINE__);

	return RunSpider::updateVersion(s, repository, prevTag, newTag, prevUnchangedFiles);

	print::loguruResetThreadName();
}

void moduleFacades::switchVersion(Spider *s, std::string tag, std::string repository)
{
	print::debug("Calling the spider to switch to version " + tag,
				 __FILE__, __LINE__);

	return RunSpider::switchVersion(s, tag, repository);

	print::loguruResetThreadName();
}

void moduleFacades::trimFiles(Spider *s, std::map<std::string, std::vector<int>> lines, std::string repository)
{
	print::debug("Calling the spider to trim the files", __FILE__, __LINE__);

	return RunSpider::trimFiles(s, lines, repository);

	print::loguruResetThreadName();
}

AuthorData moduleFacades::getAuthors(Spider *s, std::string repository)
{
	print::debug("Calling the spider to retrieve author data", __FILE__, __LINE__);

	return RunSpider::getAuthors(s, repository);

	print::loguruResetThreadName();
}

std::string moduleFacades::currentVersion(Spider *s, std::string repository)
{
	return RunSpider::getCommitHash("HEAD", repository);
}

std::vector<std::tuple<std::string, long long, std::string>> moduleFacades::getRepositoryTags(std::string downloadPath)
{
	print::debug("Calling the spider to get tags of previous versions", __FILE__, __LINE__);

	std::vector<std::tuple<std::string, long long, std::string>> tags = RunSpider::getTags(downloadPath);

	return tags;
}

std::vector<std::tuple<std::string, std::string, std::map<std::string, std::vector<int>>>>
moduleFacades::getVulnCommits(std::string downloadPath)
{
	print::debug("Calling the spider to get vulnerability commits", __FILE__, __LINE__);

	std::vector<std::tuple<std::string, std::string, std::map<std::string, std::vector<int>>>> commits =
		RunSpider::getVulns(downloadPath);

	return commits;
}

std::string moduleFacades::getVersionTime(std::string version, std::string downloadPath)
{
	print::debug("Calling the spider to get version time", __FILE__, __LINE__);

	std::string versionTime = RunSpider::getVersionTime(version, downloadPath);

	return versionTime;
}

std::vector<HashData> moduleFacades::parseRepository(std::string repository, Flags flags)
{
	print::debug("Calling the parser to parse a repository", __FILE__, __LINE__);

	if (std::filesystem::is_directory(repository))
	{
		auto hashes = Parser::parse(repository, flags.flag_cpu);
		print::loguruResetThreadName();

		return hashes;
	}
	else
	{
		return std::vector<HashData>();
	}
}

ProjectMetaData moduleFacades::getProjectMetadata(std::string url, Flags flags)
{
	print::debug("Calling the crawler to get the metadata from a project", __FILE__, __LINE__);

	ProjectMetadata pmd = RunCrawler::findMetadata(url, flags.flag_github_user, flags.flag_github_token);
	print::loguruResetThreadName();
	if (errno != 0)
	{
		print::debug("Encountered an error while retrieving the metadata", __FILE__, __LINE__);
		return ProjectMetaData();
	}

	std::string versionHash = "";

	long long hash = Utils::getIdFromPMD(pmd);

	//TD preliminary to allow debugging. To be changed when general error-handling is enhanced

	if (errno != 0)
	{
		print::debug("ERR!!!", __FILE__, __LINE__);
	}

	std::string idStr = std::to_string(hash);
	if (errno != 0)
	{
		print::debug("ERR!!!", __FILE__, __LINE__);
	}

	long long iTime = Utils::getIntegerTimeFromString(pmd.version);
	if (errno != 0)
	{
		print::debug("ERR!!!", __FILE__, __LINE__);
	}

	std::string iTimeStr = std::to_string(iTime);
	if (errno != 0)
	{
		print::debug("ERR!!!", __FILE__, __LINE__);
	}

	ProjectMetaData pm = ProjectMetaData(
		idStr, 
		iTimeStr,
		versionHash,
		pmd.license,
		pmd.name,
		pmd.url,
		pmd.authorName,
		pmd.authorMail,
		pmd.defaultBranch);
	return pm;
}

CrawlData moduleFacades::crawlRepositories(int startId, Flags flags)
{
	print::debug("Calling the crawler to crawl a repositories", __FILE__, __LINE__);

	auto urls = RunCrawler::crawlRepositories(
		"https://github.com/", 
		startId, 
		flags.flag_github_user, 
		flags.flag_github_token);
	print::loguruResetThreadName();

	return urls;
}
