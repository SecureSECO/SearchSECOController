/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "flags.h"

// Parser includes.
#include "HashData.h"

// Spider includes.
#include "CodeBlock.h"


namespace moduleFacades
{
	/// <summary>
	/// Will call the spider to download a given repository.
	/// </summary>
	AuthorData downloadRepository(std::string repository, Flags flags, std::string downloadPath);
	/// <summary>
	/// Will call the parser to parse the given repository.
	/// </summary>
	std::vector<HashData> parseRepository(std::string repository, Flags flags);

	std::vector<std::string> crawlRepositories(int startId);
};
