#ifndef SUPPORT
#define SUPPORT


#include <stdlib.h>
#include <string.h>

#include <libgen.h>


#include "globals.h"


////////////////////////////////////////////// type definitions
typedef enum filetype
{
	Filetype_Directory,
	Filetype_File
} Filetype;

typedef struct file
{
	char name[64];
	Filetype type;
	File* child;
	File* sibling;
	File* parent;
} File;


typedef enum pathtype
{
	EmptyPath,
	RelativePath,
	AbsolutePath
} PathType;


typedef struct filepath
{

	char baseName[64]; // name of file itself 
	char pathName[32][64]; // names of directory files leading to the end file
	PathType pathType;
} Filepath;


char typeToChar(Filetype type)
{
	switch(type)
	{
		case Filetype_Directory:
			return 'D';
			break;
		case Filetype_File:
			return 'F';
			break;
	}
}

Filetype charToType(char c)
{
	switch (c)
	{
		case 'D':
			return Filetype_Directory;
			break;
		case 'F':
			return Filetype_File;
			break;
	}
}

//////////////////////////////////////////// supporting functions for commands
File * allocateFile()
{
	File * newFile = malloc(sizeof(File));
	newFile->name[0] = '\0';
	newFile->type = Filetype_File;
	newFile->child = NULL;
	newFile->sibling = NULL;
	newFile->parent = NULL;
}

void freeFile(File* node)
{
	free(node);
}


// creates a root directory
File* initializeRoot()
{
	File* root = allocateFile();
	strcpy(root->name, "/");
	root->type = Filetype_Directory;
	root->sibling = NULL;
	root->parent = root;

	return root;
}

// creates a file and inserts it onto the end of the parents list of children
File* makeFile(File* parent, char* name, Filetype type)
{
	// will always be the child of something (not root)
	// just always append it to the end of the child list
	
	// set up the file to be added
	File* newFile = allocateFile();
	strcpy(newFile->name, name);
	newFile->type = type;
	newFile->parent = parent;

	// case for the parent not having children
	if (parent->child == NULL)
	{
		parent->child = newFile;
	}
	else
	{
		// find the end of the parents list of children
		File* current = parent;
		File* next = parent->child;

		while (next != NULL)
		{
			current = next;
			next = next->sibling;
		}

		// insert into the list
		current->sibling = newFile;
	}

	return newFile;
}


// remove a file from 
int removeFile(File* file)
{
	// check for children
	if (file->child != NULL) return 1;

	// check if we are root
	if (file->parent == file)
	{
		freeFile(file);
		return 0;
	}

	// check for if we are our parents starting child
	if (file->parent->child == file)
	{
		file->parent->child = file->sibling;
	}
	else
	{
		// not starting child
		// find ourselves
		File* current = file->parent->child;
		File* next = current->sibling;

		while (next != file)
		{
			current = next;
			next = next->sibling;
		}

		// current is now the file before us in the child list
		current->sibling = file->sibling;
	}

	freeFile(file);
	return 0;
}



// breaks up a filepath into its raw components
// undefined behavior for invalid filepaths
Filepath processFilepath(char * rawPath)
{
	char * s;
	char currentPath[128];
	Filepath filepath;
	memset(&filepath, 0, sizeof(Filepath));
	filepath.pathType = EmptyPath;

	// case for empty filepath
	if (strlen(rawPath) == 0) return filepath;


	// check filepath type
	if (*rawPath == '/')
	{
		filepath.pathType = AbsolutePath;
		strcpy(currentPath, rawPath+1);
	} else
	{
		filepath.pathType = RelativePath;
		strcpy(currentPath, rawPath);
	}


	// check if there is only one token in the path
	if (strstr(currentPath, "/") == NULL)
	{
		strcpy(filepath.baseName, currentPath);
		return filepath;
	}

	int i = 0;
	s = strtok(currentPath, "/"); 
	strcpy(filepath.pathName[i], s);
	i++;


	// copy directories in path
	while (s = strtok(0, "/"))
	{
		strcpy(filepath.pathName[i], s);
		i++;
	}

	if (s != NULL)
	{
		// the base is left in s, move to base name
		strcpy(filepath.baseName, s);
	}else
	{
		// the base was moved to pathName, move to base name.
		strcpy(filepath.baseName, filepath.pathName[i-1]);
		strcpy(filepath.pathName[i-1], "");
	}

	// ensure the last entry in path is an empty string
	strcpy(filepath.pathName[i-1], "");
	
	return filepath;
}

// find file
// finds a file given by its name and type immediately under the parent directory
// NOT recursive
File* findFile(char* filename, File* parentDir, Filetype fileType)
{
	File* result = NULL;
	File* currentSibling = parentDir->child;

	// check for . and .. special cases
	if (strcmp(filename, "..") == 0 && fileType == Filetype_Directory)
	{
		return parentDir->parent;
	}

	if (strcmp(filename, ".") == 0 && fileType == Filetype_Directory)
	{
		return parentDir;
	}


	// traverse through the children of the parent to find the desired file
	while (currentSibling != NULL)
	{
		if (strcmp(currentSibling->name, filename) == 0
			&& currentSibling->type == fileType)
		{
			// found the file
			result = currentSibling;
			return result;
		}

		currentSibling = currentSibling->sibling;
	}

	// could not find a file matching the criteria
	return NULL;
}

// find path directory
// returns the path directory given by a filepath
File* findPathDir(Filepath* path)
{
	File* startingFile;

	// relative or absolute?
	if (path->pathType == AbsolutePath)
	{
		startingFile = root;
	}
	else
	{
		startingFile = currentDirectory;
	}

	// follow path
	int i = 0;
	char* currentName = path->pathName[i];
	File* currentFile = startingFile;

	while (strcmp(currentName, "") != 0)
	{
		// case for ..
		if (strcmp(currentName, "..") == 0)
		{
			// move up to parent directory
			currentFile = currentFile->parent;
		}
		else if (strcmp(currentName, ".") == 0)
		{
			// case for .
			// do nothing.
		}
		else
		{
			// handle directory names
			currentFile = findFile(currentName, currentFile, Filetype_Directory);

			// directory specified by path does not exist
			if (currentFile == NULL)
			{
				return NULL;
			}
		}

		i++;
	currentName = path->pathName[i];
	}

	return currentFile;
}


// recursively deletes the current node and everything below it
void deleteFilesystem(File* root)
{
	if (root->child != NULL)
	{
		deleteFilesystem(root->child);
	}

	if (root->sibling != NULL)
	{
		deleteFilesystem(root->sibling);
	}

	removeFile(root);
}






#endif
