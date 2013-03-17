//
// FeOS C Standard Library
// sys/stat.h
//     More POSIX-like misc functions
//

#pragma once
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @file types.h
 *  \brief More POSIX-like misc functions
 */

/** @addtogroup crt_io Input/Output
 *  @{
 */

#define S_ISUID  0004000 //!< set user id on execution
#define S_ISGID  0002000 //!< set group id on execution
#define S_ISVTX  0001000 //!< save swapped text even after use
#define S_IREAD  0000400 //!< read permission, owner
#define S_IWRITE 0000200 //!< write permission, owner
#define S_IEXEC  0000100 //!< execute/search permission, owner
#define S_ENFMT  0002000 //!< enforcement-mode locking

#define S_IFMT   0170000
#define S_IFDIR  0040000
#define S_IFREG	 0100000

#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#define S_IRUSR 0000400 //!< read permission, owner
#define S_IWUSR 0000200 //!< write permission, owner
#define S_IXUSR 0000100 //!< execute/search permission, owner
#define S_IRWXG (S_IRGRP | S_IWGRP | S_IXGRP)
#define S_IRGRP 0000040 //!< read permission, group
#define S_IWGRP 0000020 //!< write permission, grougroup
#define S_IXGRP 0000010 //!< execute/search permission, group
#define S_IRWXO (S_IROTH | S_IWOTH | S_IXOTH)
#define S_IROTH 0000004 //!< read permission, other
#define S_IWOTH 0000002 //!< write permission, other
#define S_IXOTH 0000001 //!< execute/search permission, other

#define ACCESSPERMS (S_IRWXU | S_IRWXG | S_IRWXO)                               //!< 0777
#define ALLPERMS    (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO) //!< 07777
#define DEFFILEMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) //!< 0666

#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)

//! \brief File/directory status structure.
struct stat 
{
	dev_t   st_dev;
	ino_t   st_ino;
	mode_t  st_mode;
	nlink_t st_nlink;
	uid_t   st_uid;
	gid_t   st_gid;
	dev_t   st_rdev;
	off_t   st_size;
	time_t  st_atime;
	long    st_spare1;
	time_t  st_mtime;
	long    st_spare2;
	time_t  st_ctime;
	long    st_spare3;
	long    st_blksize;
	long    st_blocks;
	long    st_spare4[2];
};

int mkdir(const char*, mode_t); //!< Creates a directory.
int stat(const char*, struct stat*); //!< Gets information about a file or directory.
int fstat(int, struct stat*); //!< Gets information about a file descriptor.

/** @} */

#ifdef __cplusplus
}
#endif
