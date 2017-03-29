#ifndef GLOBALS_H
#define GLOBALS_H

#include "DataStructures.h"

#define BLKSIZE 1024


// pointers
GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

// buffers
char superBuf[BLKSIZE];
char gdBuf[BLKSIZE];


char buf[BLKSIZE];
int fd;


#endif

