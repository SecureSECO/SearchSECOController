/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

/*
NOTE: These tests depend on the used GitHub repositories. If a test fails, make sure the repository is not changed in an unexpected way.
*/

// Google test header file
#include "pch.h"

// Controller includes
#include "moduleFacades.h"
#include "projectMetadata.h"


TEST(integrationCrawler, basic)
{
	Flags flags;
	std::string url = "https://github.com/zavg/linux-0.01";
	flags.flag_github_user = "NoUser";
	flags.flag_github_token = "NoToken";
	ProjectMetaData pmd = moduleFacades::getProjectMetadata(url, flags);
	EXPECT_EQ(pmd.url, url);
	EXPECT_EQ(pmd.name, "linux-0.01");
	EXPECT_EQ(pmd.authorName, "zavg");
	EXPECT_EQ(pmd.authorMail, "");
	EXPECT_EQ(pmd.license, "");
	EXPECT_EQ(pmd.versionTime, "1380819461000");
}

TEST(integrationCrawler, incorrectURLFailurecase)
{
	Flags flags;
	std::string url = "https://secureseco.org";
	flags.flag_github_user = "user";
	flags.flag_github_token = "token";

	errno = 0;
	moduleFacades::getProjectMetadata(url, flags);
	std::cout << errno << "\n";
	EXPECT_TRUE(errno != 0);
}
