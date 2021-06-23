/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes.
#include "print.h"
#include "regexWrapper.h"
#include "utils.h"

// External includes.
#include <boost/regex.hpp>
#include <functional>


bool regex::validateSyntax(std::string callStr, std::tuple<std::string, std::string, std::string> &result)
{
	if (callStr == "") 
	{
		return false; // The empty string is by definition a syntactically malformed call string.
	}

	boost::regex expr("(?:([^-][^\\s]*)\\s?)?(?:([^-\\s][^\\s]*)\\s?)?\\s?(.*)");
	boost::smatch match;

	bool valid = boost::regex_match(callStr, match, expr);

	if (!valid)
	{
		return false;
	}

	result = std::make_tuple(match[1], match[2], Utils::trimWhiteSpaces(match[3]));
	return true;
}

void regex::parseFlargPairs(std::string flargStr, std::map<std::string, std::string> &result)
{
	std::map<std::string, std::function<void(std::string, const char*, int)>> failureExpressions =
	{
		{ "(?<!-)-([^\\s-]{2,})", error::errParseIncorrectLonghandFlag }, // -wrong.
		{ "--([^-])(?:\\s|$)", error::errParseIncorrectShorthandFlag },   // --w rong.
		{ "(-{3,}\\S+)", error::errParseCouldNotParseFlag },             // ---wrong.
	};

	std::string::const_iterator
		start = flargStr.cbegin(),
		end = flargStr.cend();

	// Check if there are any malformed flags present in the string.
	for (auto const& failureMode : failureExpressions)
	{
		boost::regex expr(failureMode.first);
		auto throwError = failureMode.second;

		boost::match_results<std::string::const_iterator> what;

		while (boost::regex_search(start, end, what, expr))
		{
			throwError(what[1], __FILE__, __LINE__);
		}
	}

	// Parse the well-formed flag-argument pairs.
	result = {};
	
	boost::regex expr("(?:(?:-([^-\\s]+)))\\s?([^-\\s]+)?");
	boost::match_results<std::string::const_iterator> what;

	while (boost::regex_search(start, end, what, expr))
	{
		std::string
			flag     = what[1],
			argument = what[2];

		result[flag] = argument;

		start = what[0].second;
	}

}

bool regex::validateURL(std::string url)
{
	// Regular expression borrowed from StackOverflow: https://stackoverflow.com/a/17773849.
	boost::regex expr("^(http:\\/\\/www\\.|https:\\/\\/www\\.|http:\\/\\/|https:\\/\\/)?[a-z0-9]+([\\-\\.]{1}[a-z0-9]+)*\\.[a-z]{2,5}(:[0-9]{1,5})?(\\/.*)?$");
	boost::smatch match;
	
	return boost::regex_match(url, match, expr);
}
