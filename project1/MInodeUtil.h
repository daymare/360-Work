#ifndef MINODE_UTIL_H
#define MINODE_UTIL_H

#include "DataStructures.h"
#include "Util.h"
#include "INodeUtil.h"

MINODE *iget(int dev, int ino);
int iput(MINODE* mip);


#endif

