#ifndef UTIL_H
#define UTIL_H


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <sys/types.h>
#include <string.h>


#include "DataStructures.h"
#include "Globals.h"



// low level utility
int get_block(int fd, int blk, char buf[ ]);
int tst_bit(char *buf, int bit);
int put_block(int fd, int blk, char buf[ ]);
int tst_bit(char *buf, int bit);
int set_bit(char *buf, int bit);
int clr_bit(char *buf, int bit);
int decFreeInodes(int dev);
int ialloc(int dev);


// parsing
void parseFilepath(char* filepath, Path* result);

// high level utility
void loadFilesystem(char* diskimage);
int checkSuper();
void loadBlocks();

// get inodes
void getInode(int index, INODE* result);
void getInodeFromBlock(char* iblock, int index, INODE* result);
void getRootInode(INODE* result);
int findFileInode(INODE* parent, char* filename, FileType type, INODE* result);
int findInodeInBlock(char* blockBuf, char* filename, FileType type, INODE* result);
int findInodePath(INODE* startingNode, char* relativeFilepath, INODE* result);
int findInodeAbsPath(char* filepath, INODE* result); 

// manipulate inodes
void printInode(INODE* inode);
void printBlock(int blockNum);
void printIndirectBlock(int blockNum, int numIndirects);


#endif
