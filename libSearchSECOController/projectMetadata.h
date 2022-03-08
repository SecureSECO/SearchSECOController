/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "networkUtils.h"

// Parser includes.
#include "Parser.h"

// External includes.
#include <string>
#include <vector>


struct ProjectMetaData
{
public:
	/// <summary>
	/// Basic constructor. All things that this constructor asks for should be provided by the spider.
	/// </summary>
	ProjectMetaData(std::string id,
		std::string versionTime,
		std::string versionHash,
		std::string license,
		std::string name,
		std::string url,
		std::string authorName,
		std::string authorMail,
		std::string defaultBranch)
	{
		this->id = id;
		this->versionTime = versionTime;
		this->versionHash = versionHash;
		this->license = license;
		this->name = name;
		this->url = url;
		this->authorName = authorName;
		this->authorMail = authorMail;
		this->defaultBranch = defaultBranch;
	}

	ProjectMetaData(){};

	/// <summary>
	/// Will call the NetworkUtils::generateHeader function with the data stored in this struct.
	/// </summary>
	std::string getAsHeader()
	{
		std::vector<std::string> header = { 
			id, 
			versionTime, 
			versionHash, 
			license, 
			name, 
			url, 
			authorName, 
			authorMail, 
			std::to_string(Parser::getHashVersion()) 
		};
		return NetworkUtils::generateHeader(header);
	};

	std::string id;
	std::string versionTime;
	std::string versionHash;
	std::string license;
	std::string name;
	std::string url;
	std::string authorName;
	std::string authorMail;
	std::string defaultBranch;
};
