#ifndef RELOAD
#define RELOAD

#include <stdio.h>

#include "../support.h"
#include "../globals.h"



int reload(char* filepath)
{
	// open file
	FILE* saveFile = fopen(filepath, "r");

	// check file exists
	if (saveFile == NULL)
	{
		printf("save file does not exist!");
		return 1;
	}

	// clear the current filesystem
	deleteFilesystem(root);

	// re initialize root
	root = initializeRoot();
	currentDirectory = root;

	// for each line in the file
	char line[256];
	int len = 0;

	memset(line, 0, 256);

	while (fgets(line, 256, saveFile) != NULL)
	{
		// clear the newline off the string
		line[strlen(line)-1] = '\0';

		// split line
		char* filetype = strtok(line, ",");
		char* path = strtok(0, ",");
		Filepath processedPath = processFilepath(path);

		// decode filetype
		Filetype type = charToType(*filetype);

		if (strcmp(path, "/") == 0)
		{
			// root line in file, ignore
			continue;
		}

		// follow path from line
		File* parent = findPathDir(&processedPath);

		// create file at path
		makeFile(parent, processedPath.baseName, type);
	}
}



#endif
