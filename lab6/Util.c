
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
    fd = open(filesystem_file, O_RDONLY);
    if (fd < 0)
    {
        printf("open failed!\n");
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

void loadBlocks()
{
    // load superblock into global memory
    get_block(fd, 1, superBuf);
    sp = (SUPER*)superBuf;

    // load group descriptor into global memory
    get_block(fd, 2, gdBuf);
    gp = (GD*)gdBuf;
}


void getInode(int index, INODE* result)
{
    // get block
    int iblock = (index-1) / 8;
    int istart = gp->bg_inode_table;
    get_block(fd, istart + iblock, buf);

    // get inode
    int inum = (index-1) % 8;
    getInodeFromBlock(buf, inum, result);
}

void getInodeFromBlock(char* iblock, int index, INODE* result)
{
    // copy inode from block into structure
    memcpy(result, (INODE*)iblock + index, sizeof(INODE));
}


void getRootInode(INODE* result)
{
    // root inode is always 2
    // get inode 2
    getInode(2, result);
}

int findFileInode(INODE* parent, char* filename, FileType type, INODE* result)
{
    // get first block in parent
    int currentBlock = parent->i_block[0];
    int found = 0;
    int i = 0;

    for (i = 0; i < 12; i++)
    {
        currentBlock = parent->i_block[i];
        if (currentBlock == 0)
        {
            printf("file not found!\n");
            return 0;
        }

        get_block(fd, currentBlock, buf);

        found = findInodeInBlock(buf, filename, type, result);

        if (found == 1)
        {
            return 1;
        }
    }

    printf("file not found!!!\n");
    return 0;

    // TODO indirect blocks
}

int findInodeInBlock(char* blockBuf, char* filename, FileType type, INODE* result)
{
    // search a block 
    // TODO potential bug, dp->name is not guaranteed to be null terminated.
    char* cp = blockBuf;
    DIR *dp = (DIR*)blockBuf;

    while (cp < &blockBuf[BLKSIZE])
    {
        if (strcmp(filename, dp->name) == 0)
        {
            if (dp->file_type == (int)type)
            {
                // get the inode of this file
                int inum = dp->inode;
                getInode(inum, result);
                return 1;
            }
            else
            {
                printf("ERROR: file is not the desired type!\n");
                return 0;
            }
        }

        cp += dp->rec_len;
        dp = (DIR*)cp;
    }

    return 0;
}

int findInodePath(INODE* startingNode, char* relativeFilepath, INODE* result)
{
    // process filepath
    Path path;
    parseFilepath(relativeFilepath, &path);

    // search for the desired node
    int i = 0;
    INODE currentNode = *startingNode;
    INODE nextNode;
    int findResult = 0;

    for (i = 0; strcmp(path.tokenizedPath[i], "") != 0; i++)
    {
        char* dirname = path.tokenizedPath[i];
        findResult = findFileInode(&currentNode, dirname, type_Directory, &nextNode);

        if (findResult == 0)
        {
            printf("file not found!");
            return 0;
        }

        currentNode = nextNode;
    }

    // get the last inode
    return findFileInode(&currentNode, path.baseName, type_File, result);
}

int findInodeAbsPath(char* filepath, INODE* result)
{
    // process filepath
    Path path;
    parseFilepath(filepath, &path);
    
    // search for the desired inode
    INODE root;
    getRootInode(&root);

    return findInodePath(&root, filepath, result);
}

// print out an inode of information in binary format
void printInode(INODE* inode)
{
    int i = 0;

    // print non-indirect blocks
    for (i = 0; i < 12; i++)
    {
        if (inode->i_block[i] == 0) return;

        printIndirectBlock(inode->i_block[i], 0);
    }

    // print indirect blocks

    if (inode->i_block[12] == 0) return;
    printIndirectBlock(inode->i_block[12], 1);
    if (inode->i_block[13] == 0) return;
    printIndirectBlock(inode->i_block[13], 2);
    if (inode->i_block[14] == 0) return;
    printIndirectBlock(inode->i_block[14], 3);
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



