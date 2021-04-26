/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

#include <string>
#include <tuple>
#include <map>

namespace regex
{
    /// <summary>
    /// Takes the user-entered "call string" as a parameter, and validates the syntactic correctness of the call.
    /// If correct, it returns a tuple containing the command, mandatory (url) argument, and the flag-argument pairs.
    /// This is returned as an out-parameter (pointer)
    /// </summary>
    bool validateSyntax(
        std::string callStr, 
        std::tuple<std::string, std::string, std::string>& result
    );

    /// <summary>
    /// Parses the flag-argument pairs from the user-entered "call string". If correct, it returns an array containing 
    /// tuples representing the flags (item 1) and arguments (item 2). It also 
    /// </summary>
    /// <param name="flargStr"></param>
    /// <param name="flargPairArray"></param>
    /// <param name="pairc"></param>
    /// <returns></returns>
    bool parseFlargPairs(
        std::string flargStr, 
        std::map<std::string, std::string> &flargPairArray
    );

    /// <summary>
    /// Validates a string is formatted as a valid URL.
    /// </summary>
    bool validateURL(std::string url);
};
