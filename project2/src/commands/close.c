

#include "../../header/commands/close.h"


int close(Command* command)
{
    // get fd
    int fileDescriptor;
    fileDescriptor = atoi(command->tokenizedCommand[1]);
    
    // verify fd is in range
    if (fileDescriptor < 0 || fileDescriptor > NFD)
    {
        return 0;
    }

    // verify fd is pointing at an OFT entry
    if (running->fd[fileDescriptor] == NULL)
    {
        return 0;
    }

    // close the file
    OFT* oftp = running->fd[fd];
    running->fd[fd] = 0;
    oftp->refCount = 0;
    if (oftp->refCount > 0) return 1;

    // file has no more refs. Close the file
    MINODE* mip;
    mip = oftp->mptr;
    iput(mip);

    return 1;
}

