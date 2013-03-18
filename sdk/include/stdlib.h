//
// FeOS C Standard Library
// stdlib.h
//     General functions
//

#pragma once
#include <stddef.h>
#include <limits.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @file stdlib.h
 *  \brief General functions
 */

/** @addtogroup crt_mem String/memory management
 *  @{
 */

// Dynamic memory handling
void* malloc(size_t); //!< Allocates the specified amount of memory.
void* memalign(size_t, size_t); //!< Allocates the specified amount of memory with the specified alignment.
void* calloc(size_t, size_t); //!< Allocates the specified number of elements of the specified size and zero-fills the memory.
void free(void*); //!< Frees a dynamically-allocated block of memory.
void* realloc(void*, size_t); //!< Reallocates a block of memory.

/** @} */

/** @addtogroup crt_misc Misc utilities
 *  @brief Miscellaneous APIs from the C standard library
 *  @code
 *  #include <stdlib.h>
 *  #include <assert.h>
 *  #include <ctype.h>
 *  #include <errno.h>
 *  #include <iso646.h>
 *  #include <limits.h>
 *  #include <setjmp.h>
 *  #include <stdarg.h>
 *  #include <stdbool.h>
 *  #include <stddef.h>
 *  #include <stdint.h>
 *  #include <time.h>
 *  @endcode
 *  @{
 */

// String to integer conversions
int atoi(const char*); //!< Convert a string to integer.
static inline long atol(const char* a) { return atoi(a); } //!< Alias for atoi().
int strtol(const char*, char**, int); //!< Convert a string to integer, with more options.
unsigned int strtoul(const char*, char**, int); //!< Convert a string to unsigned integer.

// Random number functions
#define RAND_MAX INT_MAX //!< Maximum value returned by rand().
int rand(); //!< Returns a random number.
void srand(unsigned int); //!< Seeds rand()'s random number generator.

#define EXIT_SUCCESS 0 //!< Program success exit code.
#define EXIT_FAILURE 1 //!< Program failure exit code.

// Environment
void abort() __attribute__((noreturn)); //!< Aborts the program. This function does not return.
int atexit(void (*)(void)); //!< Registers a function to be called when main() exits.
void exit(int) __attribute__((noreturn)); //!< Exits the program. This function should only be used in console-mode applications.
int system(const char* command); //!< Executes the specified command. \returns The command's exit code.

// Searching and ordering
typedef int (* c_comparator)(const void*, const void*); //!< Comparator callback.
void* bsearch(const void*, const void*, size_t, size_t, c_comparator); //!< Performs a binary search.
void qsort(void*, size_t, size_t, c_comparator); //!< Performs a quicksort.

// Absolute value
int abs(int); //!< Retrieves the absolute value of the specified integer.
static inline long labs(long a) { return abs(a); } //!< Alias for labs().

// Division
typedef struct { int quot, rem; } div_t; //!< Integer division result structure.
typedef div_t ldiv_t; //!< Alias for div_t.
div_t div(int, int); //!< Performs integer division.
static inline ldiv_t ldiv(int a, int b) { return div(a, b); } //!< Alias for div().

// These functions will never be implemented:
// atof, strtod, getenv, mblen, mbtowc, wctomb, mbstowcs, wcstombs

/** @} */

#ifdef __cplusplus
}
#endif
