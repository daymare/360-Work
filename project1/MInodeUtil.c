
#include "MInodeUtil.h"

MINODE *iget(int dev, int ino)
{
  int i, blk, disp;
  char buf[BLKSIZE];
  MINODE *mip;
  INODE *ip;
  for (i=0; i < NMINODE; i++){
    mip = &minode[i];
    if (mip->dev == dev && mip->ino == ino){
       mip->refCount++;
       printf("found [%d %d] as minode[%d] in core\n", dev, ino, i);
       return mip;
    }
  }
  for (i=0; i < NMINODE; i++){
    mip = &minode[i];
    if (mip->refCount == 0){
       printf("allocating NEW minode[%d] for [%d %d]\n", i, dev, ino);
       mip->refCount = 1;
       mip->dev = dev; mip->ino = ino;  // assing to (dev, ino)
       mip->dirty = mip->mounted = mip->mountPtr = 0;
       // get INODE of ino into buf[ ]      
       blk  = (ino-1)/8 + iblock;  // iblock = Inodes start block #
       disp = (ino-1) % 8;
       //printf("iget: ino=%d blk=%d disp=%d\n", ino, blk, disp);
       get_block(dev, blk, buf);
       ip = (INODE *)buf + disp;
       // copy INODE to mp->INODE
       mip->INODE = *ip;
       return mip;
    }
  }   
  printf("PANIC: no more free minodes\n");
  return 0;
}



