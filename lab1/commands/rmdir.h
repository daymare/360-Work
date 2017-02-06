#ifndef RMDIR
#define RMDIR

#include "../support.h"
#include "../globals.h"


// remove a directory
int rmdir(char* path)
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
	File* findResult = findFile(filepath.baseName, parentDir, Filetype_Directory);

    if (findResult == NULL)
    {
        printf("improper path!\n");
        return 1;
    }

	// make sure the base directory doesn't have any children
	if (findResult->child != NULL)
	{
		printf("directory has children!\n");
		return 2;
	}

	// remove directory
	removeFile(findResult);

	return 0;
}




#endif
