/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Controller includes
#include "flags.h"

// Parser includes
#include "HashData.h"

// Spider includes
#include "CodeBlock.h"

// External includes
#include <functional>
#include <map>
#include <string>
#include <vector>

class Command
{
public:
	/// <summary>
	/// Prints the help message corresponding to this command.
	/// </summary>
	std::string helpMessage();

	/// <summary>
	/// Will execute the given command with the flags you give it.
	/// </summary>
	/// <param name="command">The command you want to execute.</param>
	/// <param name="flags">The flags with which you want to execute the command.</param>
	virtual void execute(Flags flags) = 0;

protected:
	std::string helpMessageText;
};

class Start : public Command
{
public:
	Start();

	/// <summary>
	/// Starts the worker node.
	/// </summary>
	void execute(Flags flags) override;

	/// <summary>
	/// Logs the pre-execution message.
	/// </summary>
	static void logPreExecutionMessage(int fCPU, int fRAM, const char* file, int line);

	/// <summary>
	/// Logs the post-execution message.
	/// </summary>
	static void logPostExecutionMessage(const char* file, int line);
};

class Check : public Command
{
public:
	Check();

	/// <summary>
	/// Checks matches with the database for the given repository.
	/// </summary>
	void execute(Flags flags) override;

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
	void execute(Flags flags) override;

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
	void execute(Flags flags) override;
};

class Update : public Command
{
public:
	Update();

	/// <summary>
	/// Will update this program.
	/// </summary>
	void execute(Flags flags) override;

	/// <summary>
	/// Logs the pre-execution message.
	/// </summary>
	static void logPreExecutionMessage(std::string targetVersion, const char* file, int line);

	/// <summary>
	/// Logs the post-execution message.
	/// </summary>
	static void logPostExecutionMessage(const char* file, int line);
};
