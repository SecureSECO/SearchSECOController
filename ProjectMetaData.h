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
		std::string authorMail,
		std::string stars)
	{
		this->id = id;
		this->version = version;
		this->license = license;
		this->name = name;
		this->url = url;
		this->authorName = authorName;
		this->authorMail = authorMail;
		this->stars = stars;
	}

	std::string getAsHeader()
	{
		return NetworkUtils::generateHeader({ id, std::to_string(version), license, name, url, authorName, authorMail });
	};

	std::string id;
	std::string version;
	std::string license;
	std::string name;
	std::string url;
	std::string authorName;
	std::string authorMail;
	std::string stars;
};
