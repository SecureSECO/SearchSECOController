/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Controller includes
#include "print.h"
#include "termination.h"

// External includes
#include <iostream>

void termination::successful()
{
	print::loguruSetSilent();
	exit(0);
}

void termination::failure()
{
	print::loguruSetSilent();
	exit(1);
}

void termination::failureCrawler(const char* file, int line)
{
	print::loguruResetThreadName();
	error::errSubmoduleFatalFailureCrawler(file, line);
}

void termination::failureSpider(const char* file, int line)
{
	print::loguruResetThreadName();
	error::errSubmoduleFatalFailureSpider(file, line);
}

void termination::failureParser(const char* file, int line)
{
	print::loguruResetThreadName();
	error::errSubmoduleFatalFailureParser(file, line);
}

void termination::failureDatabase(const char* file, int line)
{
	print::loguruResetThreadName();
	error::errSubmoduleFatalFailureDatabase(file, line);
}
