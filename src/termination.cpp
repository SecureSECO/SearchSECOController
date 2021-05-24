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
