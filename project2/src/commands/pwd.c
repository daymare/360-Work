#include "../../header/commands/pwd.h"

int getParentInumber(int inumber)
{
    int result = 0;

    // find the .. directory
    DIR parentDir;
    result = getParentDir(inumber, &parentDir);

    if (result == 1)
    {
        return 0;
    }

    // extract the inumber 
    return parentDir.inode;
}


void getName(int parentInum, int inum, FileType fileType, char* nameResult)
{
    // find the directory with the correct inumber
    int result = 1;
    SearchValue value;
    value.inumber = inum;
    DIR dirResult;
    result = getDir(parentInum, value, Search_INum, fileType, &dirResult);

    // extract the name from directory structure
    memcpy(nameResult, dirResult.name, dirResult.name_len);
    nameResult[dirResult.name_len] = '\0';
}

void reverseString(char* string)
{
    int length = strlen(string);

    char* left = string;
    char* right = string + (length-1);

    while (right > left)
    {
        char temp = *left;
        *left = *right;
        *right = temp;

        right--;
        left++;
    }
}

int pwd(Command* command)
{
    char path[1024];
    char* currentPath = path;
    *currentPath = '/';
    currentPath++;

    int currentInumber = running->cwd->ino;
    int parentInumber = 0;

    // check for root
    if (currentInumber == 2)
    {
        printf("/\n");
        return 0;
    }

    char currentName[128];

    // get parent
    parentInumber = getParentInumber(currentInumber);

    // find the name of the inode in parent dir
    getName(parentInumber, currentInumber, type_Directory, currentName);

    // add to path
    strcpy(currentPath, currentName);
    currentPath += strlen(currentName);
    *currentPath = '/';
    currentPath++;

    // while parent != current
    // actually check for root inside of the loop
    while (parentInumber != currentInumber)
    {
        // set current to parent
        currentInumber = parentInumber;

        // get parent
        parentInumber = getParentInumber(currentInumber);

        // check for root
        if (currentInumber == 2)
        {
            break;
        }

        // find the name of the inode in parent dir
        getName(parentInumber, currentInumber, type_Directory, currentName);

        // add to path
        strcpy(currentPath, currentName);
        currentPath += strlen(currentName);
        *currentPath = '/';
        currentPath++;
    }

    // cap the end of the string
    *currentPath = '\0';

    // reverse the path string
    reverseString(path);

    // print out the path string
    printf("%s\n", path);

    // return 0
    return 0;
}




