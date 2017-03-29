#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
// THESE TYPES ARE IN THE (#include <ext2fs/ext2_fs2_fs.h>)
typedef struct ext2_group_desc  GD;     // give the structure a shorter name for ease of use
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

typedef enum pathtype
{
	EmptyPath,
	RelativePath,
	AbsolutePath
} PathType;

typedef struct path
{
    char tokenizedPath[10][64];
    char baseName[64];
    PathType pathType
} Path;

typedef enum fileType
{
    type_Directory = 2,
    type_Link = 7,
    type_File = 1
} FileType;




#endif

