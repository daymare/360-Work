#ifndef IUTIL_H
#define IUTIL_H

#include "Util.h"

// get inodes
void getInode(int index, INODE* result);
void getInodeFromBlock(char* iblock, int index, INODE* result);
void getRootInode(INODE* result);
int findFileInode(INODE* parent, char* filename, FileType type, INODE* result);
int findInodeInBlock(char* blockBuf, char* filename, FileType type, INODE* result);
int findInodePath(INODE* startingNode, char* relativeFilepath, INODE* result);
int findInodeAbsPath(char* filepath, INODE* result); 

// print inodes
void printInode(INODE* inode);

#endif
