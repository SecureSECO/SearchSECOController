/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// NOTE: These tests depend on the used GitHub repository. If a test fails, make sure the repository is not changed in an unexpected way.

#include "pch.h"

#include "Utils.h"
#include "ProjectMetaData.h"


TEST(IntegrationTestCrawler, BasicTest)
{
    std::string url = "https://github.com/zavg/linux-0.01";
    ProjectMetaData pmd = utils::getProjectMetadata(url);
    EXPECT_EQ(pmd.url, url);
    EXPECT_EQ(pmd.name, "linux-0.01");
    EXPECT_EQ(pmd.authorName, "zavg");
    EXPECT_EQ(pmd.authorMail, "");
    EXPECT_EQ(pmd.license, "");
    EXPECT_EQ(pmd.version, "1380815862000");
}

TEST(IntegrationTestCrawler, BasicFailureTest)
{
    std::string url = "";
    //ProjectMetaData pmd = 
    ASSERT_EXIT(utils::getProjectMetadata(url), ::testing::ExitedWithCode(EXIT_FAILURE),".*");
}

TEST(IntegrationTestCrawler, IncorrectURLFailureTest)
{
    std::string url = "https://secureseco.org";

    EXPECT_DEATH(utils::getProjectMetadata(url), ".*");
}