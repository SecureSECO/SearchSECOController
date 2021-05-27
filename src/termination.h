/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/


#pragma once
namespace termination
{
	/// <summary>
	/// Terminates the program with status code 0 (no errors).
	/// </summary>
	void successful();

	/// <summary>
	/// Terminates the program with status code 1 (failure).
	/// </summary>
	void failure();

	/// <summary>
	/// Handles retrying functionality when the Crawler failed, and terminates when we ran into a fatal failure.
	/// </summary>
	void failureCrawler(const char* file, int line);

	/// <summary>
	/// Handles retrying functionality when the Spider failed, and terminates when we ran into a fatal failure.
	/// </summary>
	void failureSpider(const char* file, int line);

	/// <summary>
	/// Handles retrying functionality when the Parser failed, and terminates when we ran into a fatal failure.
	/// </summary>
	void failureParser(const char* file, int line);

	/// <summary>
	/// Handles retrying functionality when the Database (API) failed, and terminates when we ran into a fatal failure.
	/// </summary>
	void failureDatabase(const char* file, int line);
};
