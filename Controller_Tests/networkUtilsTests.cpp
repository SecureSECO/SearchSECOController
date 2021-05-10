/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "networkUtils.h"

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
	std::string target = "HEADER\nHASH?FUNCTION?FILENAME?5?0\n";
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
	std::string target = "HEADER\nHASH?FUNCTION?FILENAME?5?1?Author?author@mail.com\n";
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
	std::string target = "HEADER\nHASH?FUNCTION?FILENAME?2?2?Author?author@mail.com?Author2?author2@mail.com\n";
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
	std::string target = "HEADER\nHASH?FUNCTION?FILENAME?5?1?Author?author@mail.com\n";
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
	std::string target = "HEADER\nHASH?FUNCTION?FILENAME?5?1?Author?author@mail.com\nHASH1?FUNCTION?FILENAME?8?1?Author?author@mail.com\nHASH2?FUNCTION?FILENAME?11?1?Author?author@mail.com\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, empty)
{
	std::vector<HashData> hashes = { };
	AuthorData authordata;
	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\n";
	EXPECT_EQ(target, std::string(buffer, buffer + size));
	EXPECT_EQ(size, target.size());
}

TEST(networkingGet_AllDataFromHashesTests, bigger)
{
	std::vector<HashData> hashes = { HashData("HASH", "FUNCTION", "FILENAME", 5, 7), HashData("HASH", "FUNCTION", "FILENAME", 2, 7), HashData("HASH", "FUNCTION", "FILENAME", 1, 7) };
	AuthorData authordata;
	int size;
	const char* buffer = NetworkUtils::getAllDataFromHashes(hashes, size, "HEADER", authordata);
	std::string target = "HEADER\nHASH?FUNCTION?FILENAME?5?0\nHASH?FUNCTION?FILENAME?2?0\nHASH?FUNCTION?FILENAME?1?0\n";
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
	std::string target = "HEADER\nHASH?FUNCTION?FILENAME?1?2?Author?author@mail.com?Author2?author2@mail.com\nHASH?FUNCTION?FILENAME?5?1?Author2?author2@mail.com\nHASH?FUNCTION?FILENAME?10?1?Author3?author3@mail.com\n";
	
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
	std::string target = "HEADER\nHASH?FUNCTION?FILENAME?5?1?Author2?author2@mail.com\nHASH?FUNCTION?FILENAME?1?2?Author?author@mail.com?Author2?author2@mail.com\nHASH?FUNCTION?FILENAME?10?1?Author3?author3@mail.com\n";

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
