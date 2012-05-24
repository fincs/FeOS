//
// FeOS C Standard Library
// unistd.h
//     POSIX-like misc functions
//

#pragma once
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

int chdir(const char*);
char* getcwd(char*, size_t);
int ftruncate(int fd, off_t length);

#ifdef __cplusplus
}
#endif
