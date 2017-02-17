#ifndef SAVE
#define SAVE

#include <stdio.h>

#include "../support.h"
#include "../globals.h"
#include "ls.h" // for typeToChar. TODO move to support

File* baseFile;


void getFilepath(File* file, char* returnPath)
{
	if (file->parent != file)
	{
		getFilepath(file->parent, returnPath);

		strcat(returnPath, file->name);

		if (file != baseFile)
		{
			strcat(returnPath, "/");
		}
	}
	else
	{
		sprintf(returnPath, "/");
	}
}


void printToFile(File* root, FILE* file)
{
	char type = typeToChar(root->type);
	char path[128] = {0};

	baseFile = root;
	getFilepath(root, path);

	fprintf(file, "%c,%s\n", type, path);
}


void save_helper(File* root, FILE* file)
{
	printToFile(root, file);

	if (root->sibling != NULL)
	{
		printToFile(root->sibling, file);
	}

	if (root->child != NULL)
	{
		printToFile(root->child, file);
	}
}

int save(char* path)
{
	File* fp = fopen(path, "w+");

	save_helper(root, fp);

	fclose(fp);
}



#endif

