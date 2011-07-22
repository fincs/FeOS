//
// FeOS C Standard Library
// unistd.h
//     POSIX-like misc functions
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

int chdir(const char*);
char* getcwd(char*, size_t);

#ifdef __cplusplus
}
#endif
