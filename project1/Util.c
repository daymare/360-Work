
#include "Util.h"
#include "DataStructures.h"

int imap, bmap;  // IMAP and BMAP block number
int ninodes, nblocks, nfreeInodes, nfreeBlocks;

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}
int put_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  write(fd, buf, BLKSIZE);
}

int tst_bit(char *buf, int bit) //This code is explained at 2.7.1
{
  int i, j;
  i = bit/8; j=bit%8;
  if (buf[i] & (1 << j)) //this shifts the buf array over to int J and checks if bit is 0 or 1. 
     return 1;
  return 0;
}

int set_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8; 
  buf[i] |= (1 << j); //this shifts the buff array over to int J and sets to bit to 1
}

int clr_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8; 
  buf[i] &= ~(1 << j); // "~" operator flips the bit over to the opposate of what it was. 1->0  and 0->1
}

// break a filepath into the individual files
void parseFilepath(char* filepath, Path* result)
{
	char * s = NULL;
	char currentPath[128];
	memset(result, 0, sizeof(Path));
	result->pathType = EmptyPath;

	// case for empty filepath
	if (strlen(filepath) == 0) return;

	// check filepath type
	if (*filepath == '/')
	{
        result->pathType = AbsolutePath;
		strcpy(currentPath, filepath+1);
	} else
	{
        result->pathType = RelativePath;
		strcpy(currentPath, filepath);
	}


	// check if there is only one token in the path
	if (strstr(currentPath, "/") == NULL)
	{
        strcpy(result->baseName, currentPath);
		return;
	}

	int i = 0;
	s = strtok(currentPath, "/"); 
    strcpy(result->tokenizedPath[i], s);
	i++;


	// copy directories in path
	while (s = strtok(0, "/"))
	{
        strcpy(result->tokenizedPath[i], s);
		i++;
	}

	if (s != NULL)
	{
		// the base is left in s, move to base name
        strcpy(result->baseName, s);
	}else
	{
		// the base was moved to pathName, move to base name.
        strcpy(result->baseName, result->tokenizedPath[i-1]);
        strcpy(result->tokenizedPath[i-1], "");
	}

	// ensure the last entry in path is an empty string
    strcpy(result->tokenizedPath[i-1], "");
}

// load a filesystem into memory
void loadFilesystem(char* filesystem_file)
{
    dev = fd = open(filesystem_file, O_RDONLY);
    if (fd < 0)
    {
        printf("open %s failed!\n", filesystem_file);
        exit(1);
    }
}


// check the super block magic number to ensure this is an EXT2 filesystem.
// this must be called after loadBlocks
int checkSuper()
{
    // check EXT2 magic number
    u16 magic = sp->s_magic;
    printf("magic = %x\n", magic);
    if (magic != 0xEF53)
    {
        printf("NOT an EXT2 Filesystem");
        return 0;
    }
    return 1;
}

// load super block and group descriptor into memory and print out important values
void loadBlocks()
{
    // load superblock into global memory
    get_block(fd, 1, superBuf);
    sp = (SUPER*)superBuf;

    ninoes = sp->s_inodes_count;
    pritnf("num inodes: %d\n", ninoes);

    nblocks = sp->s_blocks_count;
    printf("num blocks: %d\n", nblocks);

    // load group descriptor into global memory
    get_block(fd, 2, gdBuf);
    gp = (GD*)gdBuf;

    bmap = gp->bg_block_bitmap;
    printf("bmap at block: %d\n", bmap);

    imap = gp->bg_inode_bitmap;
    printf("imap at block: %d\n", imap);

    iblock = bg_inode_table;
    printf("inode table begins at block: %d\n", iblock);
}

int init()
{
    int i = 0;
    for (i = 0; i < NMINODE; i++)
    {
        minode[i].refCount = 0;
    }

    proc[0].pid = 1;
    proc[0].uid = 0;
    
    proc[1].pid = 2
    proc[1].uid = 1
}



void printIndirectBlock(int blockNum, int numIndirects)
{
    // base case, no block
    if (blockNum == 0) return;

    // base case, no indirection
    if (numIndirects < 1) return printBlock(blockNum);

    // recursive case, indirect block 
    // 256 indirections in each block
    int i = 0;
    char blkBuf[BLKSIZE];
    get_block(fd, blockNum, blkBuf);
    int* indirection = &blkBuf;

    for (i = 0; i < 256; i++)
    {
        printIndirectBlock(indirection[i], numIndirects-1);
    }
}

// print a block in binary format
// Must be fed a valid block number
void printBlock(int blockNum)
{
    get_block(fd, blockNum, buf);
    int i = 0;

    for (i = 0; i < BLKSIZE; i++)
    {
        (tst_bit(buf, i)) ? putchar('1') : putchar('0');
        if (i && (i % 8) == 0) printf(" ");
        if (i && (i % 80) == 0) printf("\n");
    }

    printf("\n");
}



