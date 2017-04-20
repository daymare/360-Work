
#include "../header/dirUtil.h"


int addDIR(MINODE *pip, int ino, char *dirName, int filetype)
{
    DIR newdir;
    char dbuf[BLKSIZE]; // buffer for directory
    char sbuf[BLKSIZE]; //buffer for string compare
    DIR *dp;
    char *cp;
    int ideal_len = 0;    //ideal length for dir entrys
    int need_len = 0;     // how much space is needed for new dir entry
    int remain_space = 0; //Use to compare space left in dir buf is enough to add new dir struct
    int blk = 0;
    int i_blockzero = 0;

    for (int x = 0; x < 12; x++)
    {
        i_blockzero = pip->INODE.i_block[x];
        if (i_blockzero == 0) //find the ith data block
        {
            i_blockzero = pip->INODE.i_block[x - 1];

            get_block(pip->dev, pip->INODE.i_block[x - 1], dbuf);

            dp = (DIR *)dbuf;
            cp = dbuf;

            while (cp + dp->rec_len < dbuf + BLKSIZE) //look for function that does this for you
            {

                cp += dp->rec_len;
                dp = (DIR *)cp;
            }

            ideal_len = 4 * ((8 + dp->name_len + 3) / 4); //find the ideal length of the last dir entry
            remain_space = dp->rec_len - ideal_len;
            int newdirnamelength = strlen(dirName);
            need_len = 4 * ((8 + newdirnamelength + 3) / 4);

            if (remain_space >= need_len) //enough space to add new dir to end of block
            {
                dp->rec_len = ideal_len; //truncate last entry to make room for new entry
                cp += dp->rec_len;  //move pointer forward so we can save new dir at end of dir entrys off by 1 error??

                /*
                        __u32 *dino = (__u32 *)cp;
                        __u16 *rlen = (__u16 *)cp + 4;
                        __u8 *nlen = (__u8 *)cp + 6;
                        char *name = (char *)cp + 8;

                        *dino = ino;
                        *rlen = remain_space;
                        *nlen = strlen(dirName);
                        strcpy(name, dirName);
                        */
                newdir.inode = ino;
                newdir.file_type = filetype;
                newdir.rec_len = remain_space;
                newdir.name_len = strlen(dirName);
                strcpy(newdir.name, dirName);
                memcpy(cp, &newdir, need_len); //if memcopy doesnt work use pointers
                put_block(dev, i_blockzero, dbuf);
                return 1;
            }
            else //need to make a new block for dir also increment parent size by 1024
            {
                int newblock = balloc();

                pip->INODE.i_block[x] = newblock;
                i_blockzero = newblock;

                get_block(dev, i_blockzero, dbuf);
                cp = dbuf;
                dp = (DIR *)dbuf;

                /*
                        __u32 *dino = (__u32 *)&dbuf;
                        __u16 *rlen = (__u16 *)&dbuf + 4;
                        __u8 *nlen = (__u8 *)&dbuf + 6;
                        char *dname = (char *)&dbuf + 8;

                        dino = ino;
                        rlen = BLKSIZE;
                        nlen = strlen(dirName);
                        strcpy(dname, dirName);
                        */

                newdir.inode = ino;
                newdir.rec_len = BLKSIZE;
                newdir.file_type = filetype;
                newdir.name_len = strlen(dirName);
                strcpy(newdir.name, dirName);
                memcpy(cp, &newdir, need_len); //if memcopy doesnt work use pointers
                put_block(dev, newblock, dbuf);
                return 1;
            }
        }
    }

    return 0;
}












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




