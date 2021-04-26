/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "regex_wrapper.h"
#include "Utils.h"

#include <regex>

bool regex::validateSyntax(std::string callStr, std::tuple<std::string, std::string, std::string> &result)
{
    if (callStr == "") 
    {
        return false; // The empty string is by definition a syntactically malformed call string.
    }

    std::regex expr("(?:([^-][^\\s]*)\\s?)?(?:([^-\\s][^\\s]*)\\s?)?\\s?(.*)");
    std::smatch match;

    bool valid = std::regex_match(callStr, match, expr);

    if (!valid)
    {
        return false;
    }

    result = std::make_tuple(match[1], match[2], utils::trimWhiteSpaces(match[3]));
    return true;
}

bool regex::parseFlargPairs(std::string flargStr, std::map<std::string, std::string> &flargPairArray)
{
    return false;
}

bool regex::validateURL(std::string url)
{
    return false;
}
