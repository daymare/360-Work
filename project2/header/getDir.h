#ifndef GET_DIR_H
#define GET_DIR_H

#include "dataStructures.h"
#include "blockUtil.h"
#include "inodeUtil.h"
#include "globals.h"

typedef enum searchType
{
    Search_Name,
    Search_INum
}SearchType;

typedef union searchValue
{
    int inumber;
    char* name;
}SearchValue;

// directory utility
int getDir(int parentInum, SearchValue searchValue, SearchType searchType, FileType fileType, DIR* dir);
int getParentDir(int inumber, DIR* result);

#endif
