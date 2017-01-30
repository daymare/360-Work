#ifndef TESTING
#define TESTING

#include <stdio.h>


#include "supportTests.h"
#include "commandTests.h"



// testing
void runTests()
{
	printf("running tests!\n");

	// install dummy filesystem
	installDummyFilesystem();
	
	// support testing
	testProcessFilepath();
	testMakeFile();
	testFindFile();

	// command testing
	testMkdir();

	// remove dummy filesystem
	removeDummyFilesystem();

	printf("all tests complete!\n");
}


#endif
