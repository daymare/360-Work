#ifndef DIR_UTIL_H
#define DIR_UTIL_H

#include "dataStructures.h"
#include "globals.h"

int addDIR(MINODE *pip, int ino, char *dirName, int filetype);
int removeDIR(int parent, SearchValue sv, SearchType st);


#endif
