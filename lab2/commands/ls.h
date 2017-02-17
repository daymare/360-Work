#ifndef LS
#define LS

#include "../support.h"

#include "../globals.h"
#include "../color.h"



void printDirectory(File* directory)
{
	File* currentFile = directory->child;

	// print out . and ..
	printf(ANSI_COLOR_GREEN "%c	%s\n" ANSI_COLOR_RESET, 'D', "..");
	printf(ANSI_COLOR_GREEN"%c	%s\n" ANSI_COLOR_RESET, 'D', ".");

	while (currentFile != NULL)
	{
		char type = typeToChar(currentFile->type);
		char* name = currentFile->name;

        if (type == 'D')
        {
		    printf(ANSI_COLOR_GREEN "%c	%s\n" ANSI_COLOR_RESET, type, name);
        }
        else
        {
		    printf("%c	%s\n", type, name);
        }

		currentFile = currentFile->sibling;
	}
}

int ls(char* path)
{
	// process filepath
	Filepath filepath = processFilepath(path);

	// check for empty filepaths
	if (filepath.pathType == EmptyPath)
	{
		// print out the current directory
		printDirectory(currentDirectory);
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
	
	// print out the final directory
	printDirectory(desiredDirectory);

	return 0;
}



#endif
