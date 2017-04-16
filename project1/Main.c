
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <ext2fs/ext2_fs.h> 


#include "Util.h"
#include "Globals.h"

int main(int argc, char*argv[])
{
    // check for a given diskimage
    if (argc > 1)
        disk = argv[1];

    // try to open the disk
    // load the filesystem
    loadFilesystem();

    printf("fd: %d\n", fd);

    // load blocks
    loadBlocks();

    // check EXT2 FS
    if (checkSuper == 0)
    {
        exit(1);
    }

    // init

    // mount root

    // create p0 as running process

    // processing loop
}


