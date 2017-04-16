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

// minodes
MINODE minode[NMINODE]; // minode array
MINODE *root; // pointer to root minode
PROC proc[NPROC];
PROC* running;

// file descriptors
int fd;
int dev;

// file system constants
int nblocks, ninoes, bmap, imap, iblock;

char* disk = "mydisk"
char line[128], cmd[64], pathname[64];



#endif

