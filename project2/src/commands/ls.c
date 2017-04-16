
#include "../../header/commands/ls.h"


int printMinodeDirectory(MINODE* directory)
{
    INODE* inode = &directory->INODE;

    int i = 0;
    for (i = 0; i < 12; i++)
    {
        printIndirectDirectory(inode->i_block[i], 0);
    }

    printIndirectDirectory(inode->i_block[12], 1);
    printIndirectDirectory(inode->i_block[13], 2);
    printIndirectDirectory(inode->i_block[14], 3);

    printf("\n");
}

int printIndirectDirectory(int blockNum, int numIndirections)
{
    // check if block number is invalid
    if (blockNum == 0) return 0;

    // no indirections
    if (numIndirections == 0)
    {
        return printDirectory(blockNum);
    }

    // one or more indirection
    int i = 0;
    char block[BLKSIZE];
    get_block(fd, blockNum, block);
    int* indirection = &block;

    for (i = 0; i < 256; i++)
    {
        printIndirectDirectory(indirection[i], numIndirections-1);
    }
}

    // print out all the names with fancy colors
int printDirectory(int blockNum)
{
    // get the block
    char block[BLKSIZE];
    get_block(fd, blockNum, block);

    // iterate through the directory block
    char* currentPlace = block;
    DIR* currentDir = (DIR*) currentPlace;

    char name[128];
    char color[5];
    int numChars = 0;

    while (currentPlace < block + BLKSIZE)
    {
        // create the directory string
        memcpy(name, currentDir->name, currentDir->name_len);
        name[currentDir->name_len] = '\0';

        // pick fancy color based on file type
        switch(currentDir->file_type)
        {
            case 1: // regular file. Boring.
                strcpy(color, ANSI_COLOR_RESET);
                break;
            case 2: // directory. Blue
                strcpy(color, ANSI_COLOR_BLUE);
                break;
            // TODO add different color for other filetypes
            defualt:
                strcpy(color, ANSI_COLOR_RESET);
                break;
        }

        // print out the entry
        printf("%s%s%s  ", color, name, ANSI_COLOR_RESET);
        numChars += currentDir->name_len;

        if (numChars > 50)
        {
            printf("\n");
            numChars = 0;
        }

        // increment our place
        currentPlace += currentDir->rec_len;
        currentDir = (DIR*) currentPlace;
    }
}

int ls(Command* command)
{
    MINODE* node;

    // is path empty? if so print out the current directory
    char* pathString = command->tokenizedCommand[1];
    if (strcmp(pathString, "") == 0)
    {
        // print current working directory of the running process
        node = running->cwd;
        printMinodeDirectory(node);
        return 0;
    }

    // get the path from the command
    Path path;
    parseFilepath(pathString, &path);

    // get the minode of the desired path
    int inodeIndex = findInodeIndex(&path, type_Directory);
    MINODE* minode = iget(dev, inodeIndex);

    // print minode
    printMinodeDirectory(minode);

    // release minode
    iput(minode);
}



