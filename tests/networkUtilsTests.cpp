/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "networkUtils.h"

// Crawler includes
#include "RunCrawler.h"

// Parser includes
#include "HashData.h"

// Spider includes
#include "CodeBlock.h"


TEST(networkingGet_AllDataFromHashesTests, basic)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7) };
	AuthorData authordata;
	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\nHASH?FUNCTION?FILENAME?5?0\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, basicAuthor)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7) };
	AuthorData authordata;
	CommitData cd1 = CommitData();
	cd1.author = "Author";
	cd1.authorMail = "author@mail.com";
	CommitData cd2 = CommitData();
	cd2.author = "Author2";
	cd2.authorMail = "author2@mail.com";
	CommitData cd3 = CommitData();
	cd3.author = "Author3";
	cd3.authorMail = "author3@mail.com";

	CodeBlock cb1 = CodeBlock();
	cb1.commit = std::make_shared<CommitData>(cd1);
	cb1.line = 4;
	cb1.numLines = 2;
	authordata["FILENAME"].push_back(cb1);

	CodeBlock cb2 = CodeBlock();
	cb2.commit = std::make_shared<CommitData>(cd2);
	cb2.line = 1;
	cb2.numLines = 2;
	authordata["FILENAME"].push_back(cb2);

	CodeBlock cb3 = CodeBlock();
	cb3.commit = std::make_shared<CommitData>(cd3);
	cb3.line = 4;
	cb3.numLines = 2;
	authordata["FILENAME2"].push_back(cb3);

	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\nHASH?FUNCTION?FILENAME?5?1?Author?author@mail.com\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, multipleAuthor)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 2, 7) };
	AuthorData authordata;
	CommitData cd1 = CommitData();
	cd1.author = "Author";
	cd1.authorMail = "author@mail.com";
	CommitData cd2 = CommitData();
	cd2.author = "Author2";
	cd2.authorMail = "author2@mail.com";
	CommitData cd3 = CommitData();
	cd3.author = "Author3";
	cd3.authorMail = "author3@mail.com";

	CodeBlock cb1 = CodeBlock();
	cb1.commit = std::make_shared<CommitData>(cd1);
	cb1.line = 4;
	cb1.numLines = 2;
	authordata["FILENAME"].push_back(cb1);

	CodeBlock cb2 = CodeBlock();
	cb2.commit = std::make_shared<CommitData>(cd2);
	cb2.line = 1;
	cb2.numLines = 2;
	authordata["FILENAME"].push_back(cb2);

	CodeBlock cb3 = CodeBlock();
	cb3.commit = std::make_shared<CommitData>(cd3);
	cb3.line = 4;
	cb3.numLines = 2;
	authordata["FILENAME2"].push_back(cb3);

	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\nHASH?FUNCTION?FILENAME?2?2?Author?author@mail.com?Author2?author2@mail.com\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, bigCommit)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7) };
	AuthorData authordata;
	CommitData cd1 = CommitData();
	cd1.author = "Author";
	cd1.authorMail = "author@mail.com";
	CommitData cd2 = CommitData();
	cd2.author = "Author2";
	cd2.authorMail = "author2@mail.com";
	CommitData cd3 = CommitData();
	cd3.author = "Author3";
	cd3.authorMail = "author3@mail.com";

	CodeBlock cb1 = CodeBlock();
	cb1.commit = std::make_shared<CommitData>(cd1);
	cb1.line = 4;
	cb1.numLines = 4;
	authordata["FILENAME"].push_back(cb1);

	CodeBlock cb2 = CodeBlock();
	cb2.commit = std::make_shared<CommitData>(cd2);
	cb2.line = 1;
	cb2.numLines = 2;
	authordata["FILENAME"].push_back(cb2);

	CodeBlock cb3 = CodeBlock();
	cb3.commit = std::make_shared<CommitData>(cd3);
	cb3.line = 4;
	cb3.numLines = 2;
	authordata["FILENAME2"].push_back(cb3);

	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\nHASH?FUNCTION?FILENAME?5?1?Author?author@mail.com\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, bigCommit2)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7), HashData("HASH1", "FUNCTION", "FILENAME", 8, 9), HashData("HASH2", "FUNCTION", "FILENAME", 11, 15) };
	AuthorData authordata;
	CommitData cd1 = CommitData();
	cd1.author = "Author";
	cd1.authorMail = "author@mail.com";

	CodeBlock cb1 = CodeBlock();
	cb1.commit = std::make_shared<CommitData>(cd1);
	cb1.line = 4;
	cb1.numLines = 16;
	authordata["FILENAME"].push_back(cb1);

	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\nHASH?FUNCTION?FILENAME?5?1?Author?author@mail.com\nHASH1?FUNCTION?FILENAME?8?1?Author?author@mail.com\nHASH2?FUNCTION?FILENAME?11?1?Author?author@mail.com\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, empty)
{
	std::vector<HashData> hashes = { };
	AuthorData authordata;
	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, bigger)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7), HashData("HASH", "FUNCTION", "FILENAME", 2, 7), HashData("HASH", "FUNCTION", "FILENAME", 1, 7) };
	AuthorData authordata;
	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\nHASH?FUNCTION?FILENAME?5?0\nHASH?FUNCTION?FILENAME?2?0\nHASH?FUNCTION?FILENAME?1?0\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, biggerAuthor)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 1, 3), HashData("HASH", "FUNCTION", "FILENAME", 5, 7), HashData("HASH", "FUNCTION", "FILENAME", 10, 12) };

	AuthorData authordata;
	CommitData cd1 = CommitData();
	cd1.author = "Author";
	cd1.authorMail = "author@mail.com";
	CommitData cd2 = CommitData();
	cd2.author = "Author2";
	cd2.authorMail = "author2@mail.com";
	CommitData cd3 = CommitData();
	cd3.author = "Author3";
	cd3.authorMail = "author3@mail.com";

	CodeBlock cb1 = CodeBlock();
	cb1.commit = std::make_shared<CommitData>(cd1);
	cb1.line = 0;
	cb1.numLines = 1;
	authordata["FILENAME"].push_back(cb1);

	CodeBlock cb2 = CodeBlock();
	cb2.commit = std::make_shared<CommitData>(cd2);
	cb2.line = 2;
	cb2.numLines = 5;
	authordata["FILENAME"].push_back(cb2);

	CodeBlock cb3 = CodeBlock();
	cb3.commit = std::make_shared<CommitData>(cd3);
	cb3.line = 11;
	cb3.numLines = 20;
	authordata["FILENAME"].push_back(cb3);

	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\nHASH?FUNCTION?FILENAME?1?2?Author?author@mail.com?Author2?author2@mail.com\nHASH?FUNCTION?FILENAME?5?1?Author2?author2@mail.com\nHASH?FUNCTION?FILENAME?10?1?Author3?author3@mail.com\n";
	
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, biggerAuthorWrongOrder)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7), HashData("HASH", "FUNCTION", "FILENAME", 1, 3), HashData("HASH", "FUNCTION", "FILENAME", 10, 12) };

	AuthorData authordata;
	CommitData cd1 = CommitData();
	cd1.author = "Author";
	cd1.authorMail = "author@mail.com";
	CommitData cd2 = CommitData();
	cd2.author = "Author2";
	cd2.authorMail = "author2@mail.com";
	CommitData cd3 = CommitData();
	cd3.author = "Author3";
	cd3.authorMail = "author3@mail.com";

	CodeBlock cb1 = CodeBlock();
	cb1.commit = std::make_shared<CommitData>(cd1);
	cb1.line = 0;
	cb1.numLines = 1;
	authordata["FILENAME"].push_back(cb1);

	CodeBlock cb2 = CodeBlock();
	cb2.commit = std::make_shared<CommitData>(cd2);
	cb2.line = 2;
	cb2.numLines = 5;
	authordata["FILENAME"].push_back(cb2);

	CodeBlock cb3 = CodeBlock();
	cb3.commit = std::make_shared<CommitData>(cd3);
	cb3.line = 11;
	cb3.numLines = 20;
	authordata["FILENAME"].push_back(cb3);

	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n\n\nHASH?FUNCTION?FILENAME?5?1?Author2?author2@mail.com\nHASH?FUNCTION?FILENAME?1?2?Author?author@mail.com?Author2?author2@mail.com\nHASH?FUNCTION?FILENAME?10?1?Author3?author3@mail.com\n";

	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, unchangedFiles)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7) };
	std::string prevCommitVersionTime = "123459";
	AuthorData authordata;
	int size;
	std::vector<std::string> unchangedFiles = { "file1","file2","file3" };
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata, prevCommitVersionTime, unchangedFiles);
	std::string target = "HEADER\n123459\nfile1?file2?file3?\nHASH?FUNCTION?FILENAME?5?0\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetHashDataFromHashesTests, basic)
{
	std::vector<HashData> hashes = { HashData("HASH0", "", "", 0, 7) };
	int size;
	const char* buffer = NetworkUtils::getHashDataFromHashes(hashes, size);
	std::string target = "HASH0\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetHashDataFromHashesTests, empty)
{
	std::vector<HashData> hashes = { };
	int size;
	const char* buffer = NetworkUtils::getHashDataFromHashes(hashes, size);
	std::string target = "";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetHashDataFromHashesTests, bigger)
{
	std::vector<HashData> hashes = { HashData("HASH0", "", "", 0, 7), HashData("HASH1", "", "", 0, 7), HashData("HASH2", "", "", 0, 7), HashData("HASH3", "", "", 0, 7) };
	int size;
	const char* buffer = NetworkUtils::getHashDataFromHashes(hashes, size);
	std::string target = "HASH0\nHASH1\nHASH2\nHASH3\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(target.size(), size);
}

TEST(networkingGetAuthorsToSend, two_authors)
{
	std::map<std::string, int> authors = { {"author1", 0}, {"author2", 0} };
	int size;
	const char* buffer = NetworkUtils::getAuthorStringToSend(authors, size);
	std::string target = "author1\nauthor2\n";
	EXPECT_EQ(size, target.size());
	EXPECT_EQ(target, std::string(buffer, buffer + size));
}

TEST(networkingGetAuthorsToSend, empty)
{
	std::map<std::string, int> authors = {  };
	int size;
	const char* buffer = NetworkUtils::getAuthorStringToSend(authors, size);
	std::string target = "";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetAuthorsToSend, more_authors)
{
	std::map<std::string, int> authors = { {"author1", 0}, {"author2", 0 }, {"author3" , 0 }, {"author4", 0}, {"author5", 0} };
	int size;
	const char* buffer = NetworkUtils::getAuthorStringToSend(authors, size);
	std::string target = "author1\nauthor2\nauthor3\nauthor4\nauthor5\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetProjectToSend, two_projects)
{
	std::set<std::pair<std::string, std::string>> projects = { {"project1", "12"}, {"project2", "51"} };
	int size;
	const char *buffer = NetworkUtils::getProjectsRequest(projects, size);
	std::string target = "project1?12\nproject2?51\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetProjectToSend, empty)
{
	std::set<std::pair<std::string, std::string>> projects = {};
	int size;
	const char *buffer = NetworkUtils::getProjectsRequest(projects, size);
	std::string target = "";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetProjectToSend, more_projects)
{
	std::set<std::pair<std::string, std::string>> projects = {{"project1", "12"},
															 {"project2", "51"},
															 {"project3", "69"},
															 {"project4", "42"},
															 {"project5", "420"}};
	int size;
	const char *buffer = NetworkUtils::getProjectsRequest(projects, size);
	std::string target = "project1?12\nproject2?51\nproject3?69\nproject4?42\nproject5?420\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetAddJobRequest, two_jobs)
{
	std::vector<std::string> authors = { "url1", "url2" };
	int size;
	const char* buffer = NetworkUtils::getJobsRequest(authors, size);
	std::string target = "url1?1\nurl2?1\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetAddJobRequest, empty)
{
	std::vector<std::string> authors = {};
	int size;
	const char* buffer = NetworkUtils::getJobsRequest(authors, size);
	std::string target = "";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetAddJobRequest, more_jobs)
{
	std::vector<std::string> authors = { "url1", "url2", "url3" , "url4" , "url5" };
	int size;
	const char* buffer = NetworkUtils::getJobsRequest(authors, size);
	std::string target = "url1?1\nurl2?1\nurl3?1\nurl4?1\nurl5?1\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetCrawledRequest, two_jobs)
{
	int crawlid = 10;
	CrawlData authors;
	authors.URLImportanceList = {std::make_tuple("url1", 2, 420000), std::make_tuple("url2", 6, 690000)};
	authors.languages = std::map<std::string, int>();
	authors.finalProjectId = crawlid;
	int size;
	const char *buffer = NetworkUtils::getUploadCrawlRequest(authors, "123", size);
	std::string target = std::to_string(crawlid) + "?123\nurl1?2?420000\nurl2?6?690000\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetCrawledRequest, empty)
{
	int crawlid = 5;
	CrawlData authors = {{}, std::map<std::string, int>(), crawlid};
	int size;
	const char *buffer = NetworkUtils::getUploadCrawlRequest(authors, "124", size);
	std::string target = std::to_string(crawlid) + "?124\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGetCrawledRequest, more_jobs)
{
	int crawlid = 25;
	CrawlData authors = {
		{{"url1", 1, 420000}, {"url2", 2, 420001}, {"url3", 3, 420002}, {"url4", 4, 420003}, {"url5", 5, 420004}},
						 std::map<std::string, int>(),
						 crawlid};
	int size;
	const char *buffer = NetworkUtils::getUploadCrawlRequest(authors, "125", size);
	std::string target = std::to_string(crawlid) + "?125\nurl1?1?420000\nurl2?2?420001\nurl3?3?420002\nurl4?4?420003\nurl5?5?420004\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingUpdateJobRequest, basic)
{
	std::string jobid = "5d514d6e-2f23-fee7-b378-feda84ec123f";
	std::string jobTime = "1634297534";
	int size;
	const char* buffer = NetworkUtils::getUpdateJobRequest(jobid, jobTime, size);
	std::string target = "5d514d6e-2f23-fee7-b378-feda84ec123f?1634297534\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingFinishJobRequest, success)
{
	std::string jobid = "5d514d6e-2f23-fee7-b378-feda84ec123f";
	std::string jobTime = "1634297534";
	int code = 0;
	std::string reason = "Success.";
	int size;
	const char* buffer = NetworkUtils::getFinishJobRequest(jobid, jobTime, code, reason, size);
	std::string target = "5d514d6e-2f23-fee7-b378-feda84ec123f?1634297534?0?Success.";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingFinishJobRequest, knownProjectError)
{
	std::string jobid = "5d514d6e-2f23-fee7-b378-feda84ec123f";
	std::string jobTime = "1634297534";
	int code = 10;
	std::string reason = "Project already known.";
	int size;
	const char* buffer = NetworkUtils::getFinishJobRequest(jobid, jobTime, code, reason, size);
	std::string target = "5d514d6e-2f23-fee7-b378-feda84ec123f?1634297534?10?Project already known.";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}