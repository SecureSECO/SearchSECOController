/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "moduleFacades.h"
#include "print.h"

// Parser includes
#include "Parser.h"

// Spider includes
#include "RunSpider.h"


AuthorData moduleFacades::downloadRepository(std::string repository, Flags flags, std::string downloadPath)
{
	print::debug("Calling the spider to download a repository", __FILE__, __LINE__);

	return RunSpider::runSpider(repository, downloadPath);
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
