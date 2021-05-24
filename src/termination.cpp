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

void termination::failureCrawler(int code, const char* file, int line)
{
	error::errSubmoduleFatalFailureCrawler(file, line);
}

void termination::failureSpider(int code, const char* file, int line)
{
	error::errSubmoduleFatalFailureSpider(file, line);
}

void termination::failureParser(int code, const char* file, int line)
{
	error::errSubmoduleFatalFailureParser(file, line);
}

void termination::failureDatabase(int code, const char* file, int line)
{
	error::errSubmoduleFatalFailureDatabase(file, line);
}
