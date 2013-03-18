//
// FeOS C Standard Library
// string.h
//     String/memory manipulation functions
//

#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @file string.h
 *  \brief String/memory manipulation functions
 */

/** @addtogroup crt_mem String/memory management
 *  @brief String/memory management API from the C standard library
 *  @code
 *  #include <stdlib.h>
 *  #include <string.h>
 *  #include <alloca.h>
 *  @endcode
 *  @{
 */

// String functions
size_t strlen(const char*); //!< Retrieves the length of a string.
char*  strcpy(char*, const char*); //!< Copies a string.
char*  strcat(char*, const char*); //!< Concatenates a string.
int    strcmp(const char*, const char*); //!< Compares two strings.
char*  strdup(const char*); //!< Duplicates a string. The return value should be eventually freed using free().
int    stricmp(const char*, const char*); //!< Compares two strings with case insensivity.
char*  strncpy(char*, const char*, size_t); //!< Copies a string into a buffer with specified size.
char*  strncat(char*, const char*, size_t); //!< Concatenates a string into a buffer with specified size.
int    strncmp(const char*, const char*, size_t); //!< Compares up to the specified number of characters of two strings.
int    strnicmp(const char*, const char*, size_t); //!< Compares up to the specified number of characters of two strings with case insensivity.

// Memory functions
void* memcpy(void*, const void*, size_t); //!< Copies memory.
void* memmove(void*, const void*, size_t); //!< Copies memory but allows for overlap.
void* memset(void*, int, size_t); //!< Fills memory.
int   memcmp(const void*, const void*, size_t); //!< Compares memory.

// Searching functions
void* memchr(const void*, int, size_t); //!< Finds the specified byte in memory.
char* strchr(const char*, int); //!< Finds the specified character in a string.
size_t strcspn(const char*, const char*); //!< TODO: description
char* strpbrk(const char*, const char*); //!< TODO: description
char* strrchr(const char*, int); //!< TODO: description
size_t strspn(const char*, const char*); //!< TODO: description
char* strstr(const char*, const char*); //!< TODO: description
char* strtok(char*, const char*); //!< TODO: description

// Misc
char* strerror(int); //!< Retrieves the string that corresponds to the specified errno code.
int strcoll(const char*, const char*); //!< TODO: description -- Dummy implementation
size_t strxfrm(char*, const char*, size_t); //!< TODO: description -- Dummy implementation

#ifdef __cplusplus
}
#endif

/** @} */
