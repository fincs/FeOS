//
// FeOS C Standard Library
// fcntl.h
//     POSIX-like I/O functions
//

#pragma once
#include <sys/types.h>
#include <stdio.h>

/** @file fcntl.h
 *  \brief POSIX-like I/O functions
 */

/** @addtogroup crt_io Input/Output
 *  @{
 */

#define O_RDONLY   0        //!< Read mode. +1 == FREAD
#define O_WRONLY   1        //!< Write mode. +1 == FWRITE
#define O_RDWR     2        //!< Read/write mode. +1 == FREAD|FWRITE
#define O_APPEND   0x0008   //!< Append (writes guaranteed at the end)
#define O_CREAT    0x0200   //!< Open with file create
#define O_TRUNC    0x0400   //!< Open with truncation
#define O_EXCL     0x0800   //!< Error on open if file exists
#define O_SYNC     0x2000   //!< Do all writes synchronously
#define O_NONBLOCK 0x4000   //!< Non blocking I/O (POSIX style) (Unsupported)
#define O_NDELAY   0x4000   //!< Non blocking I/O (4.2 style) (Unsupported)
#define O_BINARY   0x10000  //!< Binary mode (Ignored)
#define O_TEXT     0        //!< Text mode (Ignored)
#define O_NOCTTY   0x8000   //!< Don't assign a ctty on this open
#define O_ACCMODE  (O_RDONLY|O_WRONLY|O_RDWR)

#ifdef __cplusplus
extern "C"
{
#endif

int open(const char*, int, ...); //!< Opens a file. If flags & O_CREAT: (const char*, int, mode_t)
int close(int); //!< Closes a file descriptor.

ssize_t read(int, void*, size_t); //!< Reads data using a file descriptor.
ssize_t write(int, const void*, size_t); //!< Writes data using a file descriptor.
off_t lseek(int, off_t, int); //!< Changes the position of a file descriptor.

static inline off_t tell(int fd) //!< Retrieves the position of a file descriptor.
{
	return lseek(fd, 0, SEEK_CUR);
}

static inline int creat(const char* filename, mode_t mode) //!< Creates a file.
{
	return open(filename, O_CREAT | O_WRONLY | O_TRUNC, mode);
}

#ifdef __cplusplus
}
#endif

/** @} */
