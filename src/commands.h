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
#include <mutex>

class Command
{
public:
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
private:
	void handleCrawlRequest(std::vector<std::string> &splitted, Flags flags);
	void handleSpiderRequest(std::vector<std::string> &splitted, Flags flags);
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
	void execute(Flags flags) override;
};

class Upload : public Command
{
public:
	Upload();

	/// <summary>
	/// Uploads given repository.
	/// </summary>
	void execute(Flags flags) override;
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
};
