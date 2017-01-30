#ifndef GLOBALS
#define GLOBALS

#include "support.h"

typedef struct file File;


// global variables
File *root; // root directory 
File *currentDirectory; // current working directory
char inputLine[128]; // raw user input
char command[16]; // user command


#endif





