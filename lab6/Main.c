
#include "DataStructures.h"
#include "Util.h"

int main(int argc, char** argv, char** env)
{
    // check there are the right number of arguments
    if (argc != 3)
    {
        printf("incorrect number of arguments!\n");
        return -1;
    }

    // extract arguments
    char* diskimage = *(argv + 1);
    char* path = *(argv + 2);

    // load filesystem
    loadFilesystem(diskimage);
    loadBlocks();
    checkSuper();

    // get the desired inode.
    INODE desiredInode;
    int result = 0;

    result = findInodeAbsPath(path, &desiredInode);
    if (result == 0)
    {
        return 1;
    }

    // print out information
    printInode(&desiredInode);

    return 0;
}


