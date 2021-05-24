/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/


#pragma once
namespace termination
{
	void successful();

	void failureCrawler(int code);

	void failureSpider(int code);

	void failureParser(int code);

	void failureDatabase(int code);

	void failure();
};
