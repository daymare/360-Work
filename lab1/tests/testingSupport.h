#ifndef TEST_SUPPORT
#define TEST_SUPPORT

#include "../support.h"
#include "../globals.h"

File* oldRoot = NULL;
File* oldCurrent = NULL;


// makes a small dummy filesystem for testing
File* makeDummyFilesystem()
{
	File* fs = initializeRoot();
	makeFile(fs, "a_dir", Filetype_Directory);
	makeFile(fs, "b_file", Filetype_File);
	makeFile(fs->child, "c_dir", Filetype_Directory);
	makeFile(fs->child->child, "d_file", Filetype_File);

	return fs;
}

void installDummyFilesystem()
{
	// save old filesystem
	oldRoot = root;
	oldCurrent = currentDirectory;

	// install dummy fs
	File* fs = makeDummyFilesystem();
	root = fs;
	currentDirectory = fs->child->child;
}

void removeDummyFilesystem()
{
	// delete dummy filesystem
	currentDirectory = NULL;
	deleteFilesystem(root);

	// reinstall old filesystem
	root = oldRoot;
	currentDirectory = oldCurrent;
}



#endif



