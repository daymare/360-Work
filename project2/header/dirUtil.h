#ifndef DIR_UTIL_H
#define DIR_UTIL_H

#include "dataStructures.h"
#include "globals.h"

int addDIR(int parentIno, int ino, char* name);
int removeDIR(int parent, SearchValue sv, SearchType st);


#endif
