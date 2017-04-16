#ifndef BLOCK_UTIL_H
#define BLOCK_UTIL_H

#include <stdio.h>
#include <fcntl.h>

#include "dataStructures.h"
#include "globals.h"


// low level utility
int get_block(int fd, int blk, char buf[ ]);
int tst_bit(char *buf, int bit);
int put_block(int fd, int blk, char buf[ ]);
int tst_bit(char *buf, int bit);
int set_bit(char *buf, int bit);
int clr_bit(char *buf, int bit);
int decFreeInodes(int dev); // TODO implement (whatever this is)
int ialloc(int dev); // TODO implement


// high level utility
void loadFilesystem(char* diskimage); // open the disk and load file pointer into global memory
int checkSuper(); // check if the loaded filesystem is ext2
void loadBlocks(); // load important blocks



#endif
