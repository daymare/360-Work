
#include "../header/dirUtil.h"


int addDIR(int parentIno, int ino, char* name);












int findBlockWDir(int parentInum, SearchValue sv, SearchType st)
{
    // get the parent inum
    INODE parent;
    getInode(parentInum, &parent);

    // find which block contains the dir
    int i = 0; 
    int currentBlock = 0;
    int result = 0;

    DIR dummy;

    for (i = 0; i < 12; i++)
    {
        currentBlock = parent.i_block[i];
        if (currentBlock == 0)
        {
            return 0;
        }
        
        result = getDirInIndirectBlock(currentBlock, sv, st, type_Any, 0, &dummy);

        if (result == 0)
        {
            return currentBlock;
        }
    }

    result = findBlockWDirInIndirectBlock(parent.i_block[12], sv, st, 1);
    if (result != 0) return result;
    result = findBlockWDirInIndirectBlock(parent.i_block[13], sv, st, 2);
    if (result != 0) return result;
    result = findBlockWDirInIndirectBlock(parent.i_block[14], sv, st, 3);
    if (result != 0) return result;

}

int findBlockWDirInIndirectBlock(int blockNum, SearchValue sv, SearchType st, int numIndirections)
{
    // check block is valid
    if (blockNum == 0)
        return 0;

    char block[BLKSIZE];
    get_block(fd, blockNum, block);
    int result = 0;
    DIR dummy;

    // no indirection
    if (numIndirections == 0)
    {
        result = getDirInIndirectBlock(blockNum, sv, st, type_Any, 0, &dummy);
        if (result != 0) return blockNum;
    }

    int i = 0;
    int* indirection = &block;
    
    for (i = 0; i < 256; i++)
    {
        result = getDirInIndirectBlock(blockNum, sv, st, numIndirections-1);
        if (result != 0) return result;
    }

    return 0;
}

DIR* getDirPointer(char* blockBuf, SearchValue sv, SearchType st, DIR** dirBefore)
{
    char* cp = blockBuf;
    DIR* dp = (DIR*)blockBuf;
    char name[128];

    while (cp < &blockBuf[BLKSIZE])
    {
        memcpy(name, dp->name, dp->name_len);
        name[dp->name_len] = '\0';
        
        switch (st)
        {
            case Search_Name:
                if (strcmp(sv.name, name) == 0)
                {
                    // return the found DIR
                    return dp;
                }
                break;
            case Search_INum:
                if (dp->inode == sv.inumber)
                {
                    // return the found DIR
                    return dp;
                }
                break;
        }

        cp += dp->rec_len;
        *dirBefore = dp;
        dp = (DIR*)cp;
    }

    return NULL;
}

// remove the specified dir from its parent directory
// returns whether the dir was successfully removed or not.
int removeDIR(int parent, SearchValue sv, SearchType st)
{
    int result = 0;

    // find the block with the DIR we need to remove
    int resultBlock = findBlockWDir(parent, sv, st);

    if (resultBlock == 0) return 0;

    // get the block
    char block[BLKSIZE];
    get_block(fd, resultBlock, block);

    // find the dir structure
    DIR* previousDir = NULL;
    DIR* deletingDir = getDirPointer(block, sv, st, &previousDir);

    // delete the dir structure
    char* currentDir = (char*)deletingDir;
    char* nextDir = currentDir + deletingDir->rec_len;
    memset(currentDir, 0, deletingDir->rec_len);

    // check if the dir we deleted was the only dir in the block
    if (previousDir == NULL)
    {
        // deallocate the block
        // TODO deallocate the block
    }

    // check if the dir we deleted was the last dir
    if (nextDir >= block + BLKSIZE)
    {
        // update the previous dir
        previousDir->rec_len = (int)(block + BLKSIZE - (char*)previousDir);
        put_block(fd, resultBlock, block);
        return 0;
    }

    // shuffle up the remaining dirs
    while(nextDir < block + BLKSIZE)
    {
        // copy down
        DIR* nextDIR = (DIR*)nextDir;
        int recLen = nextDIR->rec_len;
        memmove(currentDir, nextDir, recLen);

        // update next pointer
        nextDir += recLen;
        currentDir += recLen;
    }

    // currentDirectory should be pointing at the last directory in the block
    DIR* currentDIR = (DIR*)currentDir;
    currentDIR->rec_len = (int)(block + BLKSIZE - currentDir);

    // put the block back into it's place
    put_block(fd, resultBlock, block);
    return 0;
}




