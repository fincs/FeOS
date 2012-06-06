//
// FeOS C Standard Library
// fcntl.h
//     POSIX-like I/O functions
//

#pragma once
#include <sys/types.h>

#define O_RDONLY   0        // +1 == FREAD
#define O_WRONLY   1        // +1 == FWRITE
#define O_RDWR     2        // +1 == FREAD|FWRITE
#define O_APPEND   0x0008   // append (writes guaranteed at the end)
#define O_CREAT    0x0200   // open with file create
#define O_TRUNC    0x0400   // open with truncation
#define O_EXCL     0x0800   // error on open if file exists
#define O_SYNC     0x2000   // do all writes synchronously
#define O_NONBLOCK 0x4000   // non blocking I/O (POSIX style)
#define O_NDELAY   0x4000   // non blocking I/O (4.2 style)
#define O_BINARY   0x10000
#define O_TEXT     0
#define O_NOCTTY   0x8000   // don't assign a ctty on this open
#define O_ACCMODE  (O_RDONLY|O_WRONLY|O_RDWR)

#ifdef __cplusplus
extern "C"
{
#endif

int open(const char*, int, ...); // if flags & O_CREAT: (const char*, int, mode_t)
int close(int);

ssize_t read(int, void*, size_t);
ssize_t write(int, const void*, size_t);
off_t lseek(int, off_t, int);

static inline off_t tell(int fd)
{
	return lseek(fd, 0, SEEK_CUR);
}

static inline int creat(const char* filename, mode_t mode)
{
	return open(filename, O_CREAT | O_WRONLY | O_TRUNC, mode);
}

#ifdef __cplusplus
}
#endif
