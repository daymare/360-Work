
#include "../../header/commands/open.h"



int myOpen(Command* command)
{
    // find path inumber
    Path filepath;
    char* stringFilepath = command->tokenizedCommand[1];
    parseFilepath(stringFilepath, &filepath);
    int inum = 0;
    inum = findInodeIndex(&filepath, type_File);

    // get open mode
    int openMode = atoi(command->tokenizedCommand[2]);

    // get minode for inumber
    MINODE* minode = iget(inum);

    // allocate new OFT
    OFT* newOFT = (OFT*)malloc(sizeof(OFT));
    newOFT->mode = openMode;
    newOFT->refCount = 1;
    newOFT->mptr = minode;

    // set OFT offset
    switch(openMode)
    {
        case 0: // read
            newOFT->offset = 0;
            break;
        case 1: // write
            truncate(minode);
            newOFT->offset = 0;
            break;
        case 2: // read-write
            newOFT->offset = 0;
            break;
        case 3: // append
            newOFT->offset = minode->INODE.i_size;
        default: // invalid mode
            printf("invalid mode!\n");
            return -1;
    }

    // set up OFT in running's fd
    int i = 0;
    for (i = 0; i < NFD; i++)
    {
        if (running->fd[i] != NULL)
        {
            running->fd[i] = newOFT;
            break;
        }
    }

    if (i = NFD)
    {
        printf("no free file descriptors in running proc!\n");
        return -1;
    }

    // update INODE time field
    unsigned int now = (unsigned)time(NULL);
    minode->INODE.i_atime = now;

    if (openMode != 0)
    {
        minode->INODE.i_mtime = now;
    }

    // return index of OFT in running's fd
    return i;
}

