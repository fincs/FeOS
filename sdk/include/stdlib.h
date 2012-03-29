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

// String to integer conversions
int atoi(const char*);
static inline long atol(const char* a) { return atoi(a); }
int strtol(const char*, char**, int);
unsigned int strtoul(const char*, char**, int);

// Random number functions
#define RAND_MAX INT_MAX
int rand();
void srand(unsigned int);

// Dynamic memory handling
void* malloc(size_t);
void* memalign(size_t, size_t);
void* calloc(size_t, size_t);
void free(void*);
void* realloc(void*, size_t);

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// Environment
void abort() __attribute__((noreturn));
//int atexit(void (*)());
void exit(int) __attribute__((noreturn));
int system(const char* command);

// Searching and ordering
typedef int (* c_comparator)(const void*, const void*);
void* bsearch(const void*, const void*, size_t, size_t, c_comparator);
void qsort(void*, size_t, size_t, c_comparator);

// Absolute value
int abs(int);
static inline long labs(long a) { return abs(a); }

// Division
typedef struct { int quot, rem; } div_t;
typedef div_t ldiv_t;
div_t div(int, int);
static inline ldiv_t ldiv(int a, int b) { return div(a, b); }

// These functions will never be implemented:
// atof, strtod, getenv, mblen, mbtowc, wctomb, mbstowcs, wcstombs

#ifdef __cplusplus
}
#endif
