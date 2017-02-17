#ifndef CREAT
#define CREAT

#include "../support.h"
#include "../globals.h"

int creat(char* path)
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

	// make sure the base file doesn't already exist
	File* findResult = findFile(filepath.baseName, parentDir, Filetype_File);
	if (findResult != NULL)
	{
		printf("file already exists!\n");
		return 2;
	}

	// create new dir
	makeFile(parentDir, filepath.baseName, Filetype_File);

	return 0;
}



#endif
