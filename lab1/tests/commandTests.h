#ifndef COMMAND_TESTS
#define COMMAND_TESTS

#include <stdio.h>

#include "testingSupport.h"
#include "../globals.h"

// include commands
#include "../commands/mkdir.h"


void testMkdir()
{
	// test making a directory from root
	{
		int result = mkdir("/test_dir");

		if (findFile("test_dir", root, Filetype_Directory) == NULL)
		{
			printf("failed to create directory!");
			exit(0);
		}

		if (result != 0)
		{
			printf("return code incorrect!");
			exit(0);
		}

		// clean up
		removeFile(findFile("test_dir", root, Filetype_Directory));
	}

	// test improper path
	{
		int result = mkdir("/does_not_exist/test_dir");

		if (result != 1)
		{
			printf("return code incorrect/did not catch improper directory!");
			exit(0);
		}
	}

	// test base directory already existing
	{
		int result = mkdir("/a_dir");

		if (result != 2)
		{
			printf("return code incorrect/did not catch improper directory!");
			exit(0);
		}
	}
}








#endif



