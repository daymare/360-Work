
#include "../../header/commands/unlink.h"


void truncatePath(Path* path)
{
    if (strcmp(path->tokenizedPath[0], "") == 0)
    {
        if (path->pathType == AbsolutePath)
        {
            strcpy(path->baseName, "");
        }
        else
        {
            strcpy(path->baseName, ".");
        }
        return;
    }

    // find the end of the tokenizedPath
    int i = 0;
    while(strcmp(path->tokenizedPath[i+1], "") != 0)
        i++;
    // i is now the last index in tokenizedPath

    // move i index to baseName
    strcpy(path->tokenizedPath[i], path->baseName);
    strcpy(path->tokenizedPath[i], "");
}

int getParentInode(Path* path, INODE* inode)
{
    Path tempPath = *path;

    // truncate the base name off the end of the path
    truncatePath(&tempPath);

    // get the parent inode
    int result = findInode(&tempPath, type_Directory, inode);

    if (result == 0)
    {
        printf("failed to get parent inode!");
        exit(1);
    }

    return result;
}

int unlink(Command* command)
{
    // get path ready for processing
    Path path;
    parseFilepath(command->tokenizedCommand[1], &path);

    // get the inode of the parent
    INODE parentInode;
    int parentIno;
    parentIno = getParentInode(&path, &parentInode);

    // update the ref count of the child inode
    INODE childInode;
    int result = 0;
    result = findFileInode(&parentInode, path.baseName, type_File, &childInode);
    
    if (result == 0)
    {
        printf("unable to find file for unlinking!\n");
        return 1;
    }

    childInode.i_links_count--;

    if (childInode.i_links_count == 0)
    {
        // free up the inode and its blocks
        // TODO
    }

    // remove the DIR entry of the child inode
    SearchValue sv;
    sv.name = path.baseName;
    result = removeDIR(parentIno, sv, Search_Name);

    return result;
}

