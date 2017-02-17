
#ifndef MKDIR
#define MKDIR


#include <stdio.h>

#include "../support.h"


int mkdir (char* path)
{
	// break up pathname into dirname and basename
	Filepath filepath = processFilepath(path);

	// search for path directory
	File* parentDir = findPathDir(&filepath);

	// failed to follow path
	if (parentDir == NULL)
	{
		printf("improper path!\n");
		return 1;
	}

	// make sure the base dir doesn't already exist
	File* findResult = findFile(filepath.baseName, parentDir, Filetype_Directory);
	if (findResult != NULL)
	{
		printf("directory already exists!\n");
		return 2;
	}

	// create new dir
	makeFile(parentDir, filepath.baseName, Filetype_Directory);

	return 0;
}

#endif
