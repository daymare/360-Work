
#include "InodeUtil.h"


void getInode(int index, INODE* result)
{
    // get block
    int iblock = (index-1) / 8;
    int istart = gp->bg_inode_table;
    get_block(fd, istart + iblock, buf);

    // get inode
    int inum = (index-1) % 8;
    getInodeFromBlock(buf, inum, result);
}

void getInodeFromBlock(char* iblock, int index, INODE* result)
{
    // copy inode from block into structure
    memcpy(result, (INODE*)iblock + index, sizeof(INODE));
}


void getRootInode(INODE* result)
{
    // root inode is always 2
    // get inode 2
    getInode(2, result);
}

int findFileInode(INODE* parent, char* filename, FileType type, INODE* result)
{
    // get first block in parent
    int currentBlock = parent->i_block[0];
    int found = 0;
    int i = 0;

    for (i = 0; i < 12; i++)
    {
        currentBlock = parent->i_block[i];
        if (currentBlock == 0)
        {
            printf("file not found!\n");
            return 0;
        }

        get_block(fd, currentBlock, buf);

        found = findInodeInBlock(buf, filename, type, result);

        if (found == 1)
        {
            return 1;
        }
    }

    printf("file not found!!!\n");
    return 0;

    // TODO indirect blocks
}

int findInodeInBlock(char* blockBuf, char* filename, FileType type, INODE* result)
{
    // search a block 
    // TODO potential bug, dp->name is not guaranteed to be null terminated.
    char* cp = blockBuf;
    DIR *dp = (DIR*)blockBuf;
    char name[128];

    while (cp < &blockBuf[BLKSIZE])
    {
        memcpy(name, dp->name, dp->name_len);
        name[dp->name_len] = '\0';

        if (strcmp(filename, name) == 0)
        {
            if (dp->file_type == (int)type)
            {
                // get the inode of this file
                int inum = dp->inode;
                getInode(inum, result);
                return 1;
            }
            else
            {
                printf("ERROR: file is not the desired type!\n");
                return 0;
            }
        }

        cp += dp->rec_len;
        dp = (DIR*)cp;
    }

    return 0;
}

int findInodePath(INODE* startingNode, char* relativeFilepath, INODE* result)
{
    // process filepath
    Path path;
    parseFilepath(relativeFilepath, &path);

    // search for the desired node
    int i = 0;
    INODE currentNode = *startingNode;
    INODE nextNode;
    int findResult = 0;

    for (i = 0; strcmp(path.tokenizedPath[i], "") != 0; i++)
    {
        char* dirname = path.tokenizedPath[i];
        findResult = findFileInode(&currentNode, dirname, type_Directory, &nextNode);

        if (findResult == 0)
        {
            printf("file not found!");
            return 0;
        }

        currentNode = nextNode;
    }

    // get the last inode
    return findFileInode(&currentNode, path.baseName, type_File, result);
}

int findInodeAbsPath(char* filepath, INODE* result)
{
    // process filepath
    Path path;
    parseFilepath(filepath, &path);
    
    // search for the desired inode
    INODE root;
    getRootInode(&root);

    return findInodePath(&root, filepath, result);
}

// print out an inode of information in binary format
void printInode(INODE* inode)
{
    int i = 0;

    // print non-indirect blocks
    for (i = 0; i < 12; i++)
    {
        if (inode->i_block[i] == 0) return;

        printIndirectBlock(inode->i_block[i], 0);
    }

    // print indirect blocks

    if (inode->i_block[12] == 0) return;
    printIndirectBlock(inode->i_block[12], 1);
    if (inode->i_block[13] == 0) return;
    printIndirectBlock(inode->i_block[13], 2);
    if (inode->i_block[14] == 0) return;
    printIndirectBlock(inode->i_block[14], 3);
}



