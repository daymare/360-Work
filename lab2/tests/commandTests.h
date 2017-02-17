#ifndef COMMAND_TESTS
#define COMMAND_TESTS

#include <stdio.h>

#include "testingSupport.h"
#include "../globals.h"

// include commands
#include "../commands/mkdir.h"
#include "../commands/rmdir.h"
#include "../commands/cd.h"


void testMkdir()
{
	// test making a directory from root
	{
		int result = mkdir("/test_dir");

		if (findFile("test_dir", root, Filetype_Directory) == NULL)
		{
			printf("failed to create directory!\n");
			exit(0);
		}

		if (result != 0)
		{
			printf("return code incorrect!\n");
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
			printf("return code incorrect/did not catch improper directory!\n");
			exit(0);
		}
	}

	// test base directory already existing
	{
		int result = mkdir("/a_dir");

		if (result != 2)
		{
			printf("return code incorrect/did not catch improper directory!\n");
			exit(0);
		}
	}
}


void testRmdir()
{
	// test removing a directory from root
	{
		mkdir("/test_dir");
		int result = rmdir("/test_dir");

		if (findFile("test_dir", root, Filetype_Directory) != NULL)
		{
			printf("failed to remove directory!\n");
			exit(0);
		}

		if (result != 0)
		{
			printf("return code incorrect!\n");
			exit(0);
		}
	}

	// test improper path
	{
		int result = mkdir("/does_not_exist/test_dir");

		if (result != 1)
		{
			printf("return code incorrect/did not catch improper directory!\n");
			exit(0);
		}
	}

	// test base directory having children
	{
		int result = rmdir("/a_dir");

		if (result != 2)
		{
			printf("return code incorrect/did not catch improper directory!\n");
			exit(0);
		}
	}

}


void testCd()
{
	File* oldCurrent = currentDirectory;

	{
		// test regular cd
		File* desiredCurrent = currentDirectory->child;
		int result = cd("e_dir");
		if (currentDirectory != desiredCurrent)
		{
			printf("cd did not change directory properly!\n");
			exit(0);
		}
		if (result != 0)
		{
			printf("return code incorrect!\n");
			exit(0);
		}

		currentDirectory = oldCurrent;
	}


	{
		// test empty filepath
		int result = cd("");
		if (currentDirectory != root)
		{
			printf("cd did not change directory properly!\n");
			exit(0);
		}
		if (result != 0)
		{
			printf("return code incorrect!\n");
			exit(0);
		}
		


		currentDirectory = oldCurrent;
	}
}








#endif



