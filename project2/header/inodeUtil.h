#ifndef IUTIL_H
#define IUTIL_H


#include "dataStructures.h"
#include "blockUtil.h"
#include "parser.h"


// get inodes
void getInode(int index, INODE* result);
void getInodeFromBlock(char* iblock, int index, INODE* result);
void getRootInode(INODE* result);
int findFileInode(INODE* parent, char* filename, FileType type, INODE* result);
int findInodeInBlock(char* blockBuf, char* filename, FileType type, INODE* result);


// TODO redo path searches to allow for commands
int findInodePath(INODE* startingNode, Path* relativeFilepath, INODE* result, FileType endType);
int findInodeAbsPath(Path* filepath, INODE* result, FileType endType); 

// get the index of an inode corresponding to the given path
int findInodeIndex(Path* filepath, FileType endType);

//Kristopher is the fucking best
int decFreeInodes();
int ialloc();
int deialloc();
int kiput(MINODE *mip);





#endif



