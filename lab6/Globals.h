#ifndef GLOBALS_H
#define GLOBALS_H

#include "DataStructures.h"

#define BLKSIZE 1024

#define NMINODE      100
#define NPROC          4

// pointers
GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

// buffers
char superBuf[BLKSIZE];
char gdBuf[BLKSIZE];

// standard buffer 
char buf[BLKSIZE];

// file descriptors
int fd;
int dev;




#endif

