/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/


#pragma once
namespace termination
{
	void successful();

	void failure();

	void failureCrawler(int code, const char* file, int line);

	void failureSpider(int code, const char* file, int line);

	void failureParser(int code, const char* file, int line);

	void failureDatabase(int code, const char* file, int line);
};
