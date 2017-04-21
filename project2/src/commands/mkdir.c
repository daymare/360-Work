#include "../../header/commands/mkdir.h"

int mkDir(Command* command)
{
    MINODE *mip = NULL;
    MINODE *pip = NULL;
    int checkforName = 1;
    int pino = 0, i = 0;
    char *child = NULL;
    char temp[64] = {0};
    checkforName = search(dev, 0);
    if(checkforName != 0)
    {
        printf("Dirname already exitst!\n");
        return 0;
    }
    kpathname[i] = kpath;
    for(i = 0; kpathname[i] !='\0'; i++)
    {
        child = kpathname[i];
    }
    if( i != 1)
    {
        kpathname[i - 1] = '\0';
    }
    if (kpath[0] == '/')
    {
        mip = root;
        dev = root->dev;
        pino = search(dev, 0);
    }
    else
    {
        mip = running->cwd;
        dev = running->cwd->dev;
        pino = running->cwd->ino;
    }
    pip = iget(dev, pino);

    mymkdir(pip, child);

    pip->refCount = 1;
    pip->dirty++;
    //pip->INODE.i_atime = i_ctime = i_mtime = time(0L);
   iput(pip);
}

int mymkdir(MINODE *pip, char *name)
{
    MINODE *mip = NULL;
    int ino = ialloc();
    int bno = balloc();
    char dbuf[BLKSIZE] = {0};

    mip = iget(dev, ino);
    INODE *ip = &mip->INODE;
    ip->i_mode = 040755;      // OR 040755: DIR type and permissions
    ip->i_uid = running->uid; // Owner uid
    //ip->i_gid = running->gid;                   // Group Id
    ip->i_size = BLKSIZE;  // Size in bytes
    ip->i_links_count = 2; // Links count=2 because of . and ..
    //ip->i_atime = i_ctime = i_mtime = time(0L); // set to current time
    ip->i_blocks = 2; // LINUX: Blocks count in 512-byte chunks
    ip->i_block[0] = bno;
    ip->i_size_high = 0;
    for (int i = 1; i < 15; i++)
    {
        ip->i_block[i] = 0;
    }

    mip->dirty = 1; // mark mINODE dirty
    iput(mip);      // write INODE to disk

    char *buffptr = dbuf;
    DIR rootdir;
    rootdir.rec_len = 12;
    rootdir.name_len = 1;
    rootdir.inode = ino;
    rootdir.file_type = EXT2_FT_DIR;
    strcpy(rootdir.name, ".");
    memcpy(buffptr, &rootdir, 12);

    buffptr += 12;

    DIR parentdir;
    parentdir.rec_len = 1012;
    parentdir.name_len = 2;
    parentdir.inode = pip->ino;
    parentdir.file_type = EXT2_FT_DIR;
    strcpy(parentdir.name, "..");
    memcpy(buffptr, &parentdir, 12); //if memcopy doesnt work use pointers

    put_block(dev, bno, dbuf);

    addDIR(pip, ino, name, EXT2_FT_DIR );
}

int creat_file(Command* command)
{
    MINODE *mip = NULL;
    MINODE *pip = NULL;
    int pino = 0, i = 0;
    char *child = NULL;
    char temp[64] = {0};

    for(i = 0; kpathname[i] !='\0'; i++)
    {
        child = kpathname[i];
    }
    if( i != 1)
    {
        kpathname[i - 1] = '\0';
    }
    if (kpath[0] == '/')
    {
        mip = root;
        dev = root->dev;
        pino = search(dev, 0);
    }
    else
    {
        mip = running->cwd;
        dev = running->cwd->dev;
        pino = running->cwd->ino;
    }
    pip = iget(dev, pino);

    my_creat(pip, child);

    pip->refCount += 1;
    pip->dirty += 1;
    //pip->INODE.i_atime = i_ctime = i_mtime = time(0L);
    iput(pip);
}

int my_creat(MINODE *pip, char *name)
{
    MINODE *mip = NULL;
    int ino = ialloc();
    char dbuf[BLKSIZE] = {0};

    mip = iget(dev, ino);
    ip = &mip->INODE;
    INODE *ip = &mip->INODE;
    ip->i_mode = 0100644;    
    ip->i_uid = running->uid; // Owner uid
    //ip->i_gid = running->gid;                   // Group Id
    ip->i_size = 0;  // Size in bytes
    ip->i_links_count = 1; // Links count=2 because of . and ..
    //ip->i_atime = i_ctime = i_mtime = time(0L); // set to current time
    ip->i_blocks = 0; // LINUX: Blocks count in 512-byte chunks
    ip->i_size_high = 0;

    iput(mip);
    mip->dirty = 1; // mark mINODE dirty
    addDIR(pip, ino, name, EXT2_FT_REG_FILE);
    iput(mip);      // write INODE to disk
}