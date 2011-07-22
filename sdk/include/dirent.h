//
// FeOS C Standard Library
// dirent.h
//     POSIX-like directory functions
//

#pragma once
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define	DT_UNKNOWN	 0
#define	DT_DIR		 4
#define	DT_REG		 8

typedef struct
{
} DIR;

struct dirent
{
	ino_t d_ino;
	unsigned char d_type;
	char d_name[NAME_MAX+1];
};

DIR* opendir(const char*);
int closedir(DIR*);
struct dirent* readdir(DIR*);
void rewinddir(DIR*);
void seekdir(DIR*, int);
int telldir(DIR*);

#ifdef __cplusplus
}
#endif
