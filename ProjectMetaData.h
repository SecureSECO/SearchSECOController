/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#pragma once
#include <vector>
#include <string>
#include "NetworkUtils.h"

struct ProjectMetaData
{
public:
	ProjectMetaData(std::string id,
		std::string version,
		std::string license,
		std::string name,
		std::string url,
		std::string authorName,
		std::string authorMail)
	{
		this->id = id;
		this->version = version;
		this->license = license;
		this->name = name;
		this->url = url;
		this->authorName = authorName;
		this->authorMail = authorMail;;
	}

	std::string getAsHeader()
	{
		return NetworkUtils::generateHeader({ id, version, license, name, url, authorName, authorMail });
	};

	std::string id;
	std::string version;
	std::string license;
	std::string name;
	std::string url;
	std::string authorName;
	std::string authorMail;
};
