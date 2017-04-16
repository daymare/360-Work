#include "../header/getDir.h"

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

int getDir(int parentInum, char* name, FileType fileType, DIR* dir)
{
    // get parent inode
    INODE parent;
    getInode(parentInum, &parent);

    // search through each directory
    int i = 0;
    int currentBlock = 0;
    int result = 0;

    for (i = 0; i < 12; i++)
    {
        currentBlock = parent.i_block[i];
        if (currentBlock == 0)
        {
            printf("dir not found!\n");
            return 1;
        }

        result = getDirInIndirectBlock(currentBlock, name, fileType, 0, dir);

        if (result == 0)
        {
            return 0;
        }
    }

    result = getDirInIndirectBlock(parent.i_block[12], name, fileType, 1, dir);
    if (result == 0) return result;
    result = getDirInIndirectBlock(parent.i_block[13], name, fileType, 2, dir);
    if (result == 0) return result;
    result = getDirInIndirectBlock(parent.i_block[14], name, fileType, 3, dir);
    if (result == 0) return result;

    return 1;
}

int getDirInIndirectBlock(int blockNum, char* name, FileType fileType, int numIndirections, DIR* dir)
{
    // valid block?
    if (blockNum == 0) 
        return 1;

    char block[BLKSIZE];
    get_block(fd, blockNum, block);

    // no indirection
    if (numIndirections == 0) 
    {
        return getDirInBlock(block, name, fileType, dir);
    }

    // one or more indirections
    int i = 0;
    int* indirection = &block;
    int result = 0;

    for (i = 0; i < 256l; i++)
    {
         result = getDirInIndirectBlock(indirection[i], name, fileType, numIndirections-1, dir);

         if (result == 0)
         {
            return 0;
         }
    }

    return 1;
}

int getDirInBlock(char* blockBuf, char* filename, FileType fileType, DIR* result)
{
    char* cp = blockBuf;
    DIR* dp = (DIR*)blockBuf;
    char name[128];

    while (cp < &blockBuf[BLKSIZE])
    {
        memcpy(name, dp->name, dp->name_len);
        name[dp->name_len] = '\0';

        if (strcmp(filename, name) == 0)
        {
            if (dp->file_type == (int)fileType)
            {
                // return the found DIR
                *result = *dp;
                return 0;
            }
            else
            {
                printf("ERROR: file is not the desired type!\n");
                return 1;
            }
        }

        cp += dp->rec_len;
        dp = (DIR*)cp;
    }

    return 1;
}

int getParentDir(int inumber, DIR* result)
{
    return getDir(inumber, "..", type_Directory, result);
}

