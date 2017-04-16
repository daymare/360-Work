
#include "../header/inodeUtil.h"


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
    int index = 0;
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

        index = findInodeInBlock(buf, filename, type, result);

        if (index != 0)
        {
            return index;
        }
    }

    printf("file not found!!!\n");
    return 0;

    // TODO indirect blocks
}


// returns the index of the inode in a block
int findInodeIndexInBlock(char* blockBuf, char* filename, FileType type)
{
    // search a block 
    char* cp = blockBuf;
    DIR *dp = (DIR*)blockBuf;
    char name[128];

    while (cp < &blockBuf[BLKSIZE])
    {
        memcpy(name, dp->name, dp->name_len);
        name[dp->name_len] = '\0';

        if (strcmp(filename, name) == 0)
        {
            if (dp->file_type == (int)type)
            {
                // get the inode of this file
                int inum = dp->inode;
                return inum;
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


int findInodeInBlock(char* blockBuf, char* filename, FileType type, INODE* result)
{
    int index = findInodeIndexInBlock(blockBuf, filename, type);
    if (index != 0) 
        getInode(index, result);
    return index;
}

int findInodePath(INODE* startingNode, Path* path, INODE* result, FileType endType)
{
    // search for the desired node
    int i = 0;
    INODE currentNode = *startingNode;
    INODE nextNode;
    int findResult = 0;

    for (i = 0; strcmp(path->tokenizedPath[i], "") != 0; i++)
    {
        char* dirname = path->tokenizedPath[i];
        findResult = findFileInode(&currentNode, dirname, type_Directory, &nextNode);

        if (findResult == 0)
        {
            printf("file not found!");
            return 0;
        }

        currentNode = nextNode;
    }

    // get the last inode
    return findFileInode(&currentNode, path->baseName, endType, result);
}

int findInodeAbsPath(Path* filepath, INODE* result, FileType endType)
{
    // search for the desired inode
    INODE root;
    getRootInode(&root);

    return findInodePath(&root, filepath, result, endType);
}

int findInodeIndex(Path* filepath, FileType endType)
{
    INODE dummyInode;
    // absolute or relative?
    if (filepath->pathType == AbsolutePath)
    {
        // return absolute path result
        return findInodeAbsPath(filepath, &dummyInode, endType);
    }
    else
    {
        // return relative path result
        return findInodePath(&(running->cwd->INODE), filepath, &dummyInode, endType);
    }
}


