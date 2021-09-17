/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes.
#include "dto.h"
#include "flags.h"
#include "projectMetadata.h"

// Spider includes.
#include "Spider.h"

// External includes.
#include <mutex>

#define warnAndReturnIfErrno(message) if (errno != 0){errno = 0;print::warn(message, __FILE__, __LINE__);return;}


class Command
{
public:
	/// <summary>
	/// Returns the help message corresponding to this command.
	/// </summary>
	std::string helpMessage();

	/// <summary>
	/// Will execute the given command with the flags you give it.
	/// </summary>
	/// <param name="flags">The flags with which you want to execute the command.</param>
	virtual void execute(
		Flags flags, 
		EnvironmentDTO *env
		) = 0;

protected:
	std::string helpMessageText;

	/// <summary>
	/// Parses and blames local repository.
	/// </summary>
	/// <param name="s"> The Spider to use. </param>
	/// <param name="flags"> Flags to use. </param>
	/// <returns> Tuple of parsed HashData and AuthorData. </returns>
	std::tuple<std::vector<HashData>, AuthorData> parseAndBlame(Spider *s, Flags flags);

	/// <summary>
	/// Processes local project and uploads extracted methods to the database.
	/// </summary>
	/// <param name="s"> The Spider to use. </param>
	/// <param name="flags"> Flags to use. </param>
	/// <param name="meta"> Metadata for the local project. </param>
	/// <param name="jobid"> JobID of the current job. </param>
	/// <param name="jobTime"> JobTime for the current job. </param>
	void uploadProject(Flags flags, std::string jobid, std::string &jobTime, EnvironmentDTO *env);

	/// <summary>
	/// Processes project and compares it to the database.
	/// </summary>
	/// <param name="flags"> Flags to use. </param>
	void checkProject(Flags flags, EnvironmentDTO *env);

private:
	
	/// <summary>
	/// Used by version processing. Will parse and upload the latest version of the project.
	/// </summary>
	void parseLatest(Spider *s, ProjectMetaData &meta, Flags &flags, EnvironmentDTO *env);

	/// <summary>
	/// Loops through all tags of a project and calls downloadTagged for each of them.
	/// </summary>
	void loopThroughTags(Spider *s, std::vector<std::tuple<std::string, long long, std::string>> &tags,
						 ProjectMetaData &meta, long long startingTime, Flags &flags, std::string jobid, std::string &jobTime, EnvironmentDTO *env);

	/// <summary>
	/// Parses and uploads a single tag of a repository.
	/// </summary>
	/// <param name="s"> The Spider to use. </param>
	/// <param name="flags"> Flags to use. </param>
	/// <param name="prevTag"> The tag we parsed before this. </param>
	/// <param name="curTag"> The tag we are parsing. </param>
	/// <param name="meta"> The meta data for the repository. </param>
	/// <param name="prevVersionTime"> The time for the previous tag. </param>
	void downloadTagged(Spider *s, Flags flags, std::string prevTag, std::string curTag, ProjectMetaData meta,
						std::string prevVersionTime, std::vector<std::string> &prevUnchangedFiles, EnvironmentDTO *env);
};

class Start : public Command
{
public:
	Start();

	/// <summary>
	/// Starts the worker node.
	/// </summary>
	void execute(
		Flags flags, 
		EnvironmentDTO *env
		) override;

	/// <summary>
	/// Logs the pre-execution message.
	/// </summary>
	static void logPreExecutionMessage(int fCPU, const char* file, int line);

	/// <summary>
	/// Logs the post-execution message.
	/// </summary>
	static void logPostExecutionMessage(const char* file, int line);
private:
	/// <summary>
	/// Handles crawl requests.
	/// </summary>
	void handleCrawlRequest(
		std::vector<std::string> &splitted, 
		Flags flags, 
		EnvironmentDTO *env);

	/// <summary>
	/// Handles a spider request. 
	/// Will spider the given job and parse all tags in the repository by calling downloadTagged.
	/// </summary>
	void versionProcessing(std::vector<std::string> &splitted, Flags flags, EnvironmentDTO* env);

	/// <summary>
	/// Reads the command line.
	/// </summary>
	void readCommandLine();
	bool stop = false;
	std::mutex mtx;
};

class Check : public Command
{
public:
	Check();

	/// <summary>
	/// Checks matches with the database for the given repository.
	/// </summary>
	void execute(
		Flags flags, 
		EnvironmentDTO *env
		) override;

	/// <summary>
	/// Logs the pre-execution message.
	/// </summary>
	static void logPreExecutionMessage(std::string url, const char* file, int line);

	/// <summary>
	/// Logs the post-execution message.
	/// </summary>
	static void logPostExecutionMessage(std::string url, const char* file, int line);

private:
	/// <summary>
	/// Constructs the partial message to be logged. An appropriate prefix will be prepended to this.
	/// </summary>
	static std::string partialLogMessage(std::string url);
};

class Upload : public Command
{
public:
	Upload();

	/// <summary>
	/// Uploads given repository.
	/// </summary>
	void execute(
		Flags flags, 
		EnvironmentDTO *env
		) override;

	/// <summary>
	/// Logs the pre-execution message.
	/// </summary>
	static void logPreExecutionMessage(std::string url, const char* file, int line);

	/// <summary>
	/// Logs the post-execution message.
	/// </summary>
	static void logPostExecutionMessage(std::string url, const char* file, int line);

private:
	/// <summary>
	/// Constructs the partial message to be logged. An appropriate prefix will be prepended to this.
	/// </summary>
	static std::string partialLogMessage(std::string url);
};

class CheckUpload : public Command
{
public:
	CheckUpload();

	/// <summary>
	/// Does both check and upload for a given repository.
	/// </summary>
	void execute(
		Flags flags,
		EnvironmentDTO *env
		) override;
};
