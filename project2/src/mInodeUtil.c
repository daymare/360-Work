#include "../header/mInodeUtil.h"

MINODE *iget(int dev, int ino)
{
    int i, blk, disp;
    char dbuf[BLKSIZE];
    MINODE *mip;
    INODE *ip;
    for (i = 0; i < NMINODE; i++)
    {
        mip = &minode[i];
        if (mip->dev == dev && mip->ino == ino)
        {
            mip->refCount++;
            printf("found [%d %d] as minode[%d] in core\n", dev, ino, i);
            return mip;
        }
    }
    for (i = 0; i < NMINODE; i++)
    {
        mip = &minode[i];
        if (mip->refCount == 0)
        {
            printf("allocating NEW minode[%d] for [%d %d]\n", i, dev, ino);
            mip->refCount = 1;
            mip->dev = dev;
            mip->ino = ino; // assing to (dev, ino)
            mip->dirty = mip->mounted = mip->mptr = 0;
            // get INODE of ino into dbuf[ ]
            blk = (ino - 1) / 8 + iblock; // iblock = Inodes start block #
            disp = (ino - 1) % 8;
            //printf("iget: ino=%d blk=%d disp=%d\n", ino, blk, disp);
            get_block(dev, blk, dbuf);
            ip = (INODE *)dbuf + disp;
            // copy INODE to mp->INODE
            mip->INODE = *ip;
            return mip;
        }
    }
    printf("PANIC: no more free minodes\n");
    return 0;
}

// remove a reference to an minode
int iput(MINODE *mip)
{
    if (mip->refCount > 0)
    {
        mip->refCount--;

        if (mip->refCount == 0)
        {
            iwrite(mip);
        }
    }
}

// write an minode back to disk
// WARNING: does not care about the minode ref count
// TODO modify to care about where this was mounted
int iwrite(MINODE *mip)
{
    int blk = 0;
    int offset = 0;
    mip->refCount = 0;

    if (mip->dirty)
    {
        // find block number
        int blockNum = mip->ino;

        blk = (((blockNum)-1) / 8) + iblock;
        offset = ((blockNum)-1) % 8;

        char inodeBlock[BLKSIZE];
        get_block(dev, blk, inodeBlock);
        
        char *inodeptr = (INODE *)buf + offset;

        // write back to that block number
        memcpy(inodeptr, &(mip->INODE), sizeof(INODE));
        put_block(fd, blk, inodeBlock);
    }
}
