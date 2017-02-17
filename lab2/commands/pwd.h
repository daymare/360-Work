#ifndef PWD
#define PWD

#include <stdio.h>

#include "../globals.h"
#include "../support.h"

File* baseFile;

void pwdHelper(File* directory)
{
	if (directory->parent != directory)
	{
		pwdHelper(directory->parent);
		printf("%s", directory->name);

		if (directory != baseFile)
		{
			printf("/");
		}
	}
	else
	{
		printf("/");
	}
}


int pwd(char* path)
{
	baseFile = currentDirectory;
	pwdHelper(currentDirectory);
}


#endif




