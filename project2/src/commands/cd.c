
#include "../../header/commands/cd.h"

int cd(Command* command)
{
    // assume command is valid
    // TODO have parser take care of this.

    MINODE* minode;

    // path empty? go to root.
    char* pathString = command->tokenizedCommand[1];
    if (strcmp(pathString, "") == 0)
    {
        minode = iget(dev, 2);

        running->cwd = minode;
    }

    // get the path from the command
    Path path;
    parseFilepath(pathString, &path);

    // find inode
    int inodeIndex = findInodeIndex(&path, type_Directory);
    minode = iget(dev, inodeIndex);
    if(&(minode->INODE) == 0)
    {
        printf("iget failed to return correct address!\n");
        exit(1);
    }
    running->cwd = minode;

    return 0;
}





