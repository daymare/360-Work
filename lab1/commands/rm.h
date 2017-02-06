#ifndef RM
#define RM


#include "../support.h"
#include "../globals.h"


// remove a file
int rm(char* path)
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

	// get the pointer to the file from the file tree
	File* findResult = findFile(filepath.baseName, parentDir, Filetype_File);

	// remove directory
	removeFile(findResult);

	return 0;
}



#endif


