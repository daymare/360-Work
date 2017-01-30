#ifndef SUPPORT_TESTS
#define SUPPORT_TESTS

#include <stdio.h>
#include <stdlib.h>


#include "testingSupport.h"

#include "../support.h"
#include "../globals.h"


// test process filepath
void testProcessFilepath()
{
	// test "normal" cases
	char normalFilepath1[64] = "./a/b/c";

	Filepath resultingFilepath1 = processFilepath(normalFilepath1);
	if (strcmp(resultingFilepath1.baseName, "c") != 0) 
	{
		printf("incorrect base name!");
		exit(0);
	}
	if (strcmp(resultingFilepath1.pathName[0], ".") != 0) 
	{
		printf("incorrect path name!");
		exit(0);
	}
	if (strcmp(resultingFilepath1.pathName[1], "a") != 0) 
	{
		printf("incorrect path name!");
		exit(0);
	}
	if (strcmp(resultingFilepath1.pathName[2], "b") != 0) 
	{
		printf("incorrect path name!");
		exit(0);
	}
	if (resultingFilepath1.pathType != RelativePath) 
	{
		printf("incorrect path type!");
		exit(0);
	}


	// test absolute path
	char absoluteFilepath[64] = "/a";
	Filepath absoluteResult = processFilepath(absoluteFilepath);
	if (strcmp(absoluteResult.baseName, "a") != 0) 
	{
		printf("incorrect base name!");
		exit(0);
	}
	if (absoluteResult.pathType != AbsolutePath) 
	{
		printf("incorrect path type!");
		exit(0);
	}

	// test case where there is a / at the end
	char slashEndFilepath[64] = "./";
	Filepath resultingFilepath = processFilepath(slashEndFilepath);
	if (strcmp(resultingFilepath.baseName, ".") != 0) 
	{
		printf("incorrect base name!");
		exit(0);
	}
}

void testMakeFile()
{
	File* fs = initializeRoot();

	File* testDir = makeFile(fs, "testdir", Filetype_Directory);
	if (fs->child != testDir) 
	{
		printf("incorrect fs child!");
		exit(0);
	}

	File* testFile = makeFile(fs, "testFile", Filetype_File);
	if (testDir->sibling != testFile) 
	{
		printf("incorrect sibling!");
		exit(0);
	}
}


void testFindFile()
{
	File* fs = makeDummyFilesystem();

	File* result = findFile("b_file", fs, Filetype_File);
	if (result != fs->child->sibling) 
	{
		printf("incorrect file found!");
		exit(0);
	}
}

void testFindPathDir()
{
	// remember home directory is c_dir for dummy filesystem
	// dummy filesystem is:
	// /
	// a_dir, b_file
	// c_dir
	// d_file

	{ // scope variables so we can reuse names
	// test relative path
	Filepath path = processFilepath("d_file");
	File* correctFile = currentDirectory->child;
	File* givenFile = findPathDir(&path);
	if (correctFile != givenFile) 
	{
		printf("incorrect file given!");
		exit(0);
	}
	}


	{
	// test absolute path
	Filepath path = processFilepath("/b_file");
	File* correctFile = root->child->sibling;
	File* givenFile = findPathDir(&path);
	if (correctFile != givenFile) 
	{
		printf("incorrect file given!");
		exit(0);
	}
	}
}


#endif
