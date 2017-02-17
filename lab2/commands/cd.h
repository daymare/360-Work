#ifndef CD
#define CD

#include "../support.h"

int cd(char* path)
{
	// process filepath
	Filepath filepath = processFilepath(path);

	// check for empty filepaths
	if (filepath.pathType == EmptyPath)
	{
		currentDirectory = root;
		return 0;
	}
	
	// search for path directory
	File* parentDir = findPathDir(&filepath);

	// failed to follow path
	if (parentDir == NULL)
	{
		printf("improper path!\n");
		return 1;
	}

	// find the final directory
	File* desiredDirectory = findFile(filepath.baseName, parentDir, Filetype_Directory);
	if (desiredDirectory == NULL)
	{
		printf("improper path!\n");
		return 2;
	}

	// move to final directory
	currentDirectory = desiredDirectory;
	return 0;
}



#endif



