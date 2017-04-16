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
void loadFilesystem(char* diskimage); // open the disk and load file pointer into global memory
int checkSuper(); // check if the loaded filesystem is ext2
void loadBlocks(); // load important blocks

int init(); // initialize minodes

// print blocks
void printBlock(int blockNum);
void printIndirectBlock(int blockNum, int numIndirects);


#endif
