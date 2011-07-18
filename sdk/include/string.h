//
// FeOS C Standard Library
// string.h
//     String/memory manipulation functions
//
// These small/fast/common functions are statically linked
// to the executable for performance reasons.
//

#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

// String functions
size_t strlen(const char*);
char*  strcpy(char*, const char*);
char*  strcat(char*, const char*);
int    strcmp(const char*, const char*);
char*  strncpy(char*, const char*, size_t);
char*  strncat(char*, const char*, size_t);
int    strncmp(const char*, const char*, size_t);

// Memory functions
void* memcpy(void*, const void*, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
int   memcmp(const void*, const void*, size_t);

// TODO: Searching functions

// These functions will never be implemented:
// strcoll, strxfrm, strerror

#ifdef __cplusplus
}
#endif
