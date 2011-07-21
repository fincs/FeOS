//
// FeOS C Standard Library
// stdlib.h
//     General functions
//

#pragma once
#include <stddef.h>
#include <limits.h>
#include <feosuser.h>

#ifdef __cplusplus
extern "C"
{
#endif

// String to integer conversions
#define atol atoi
int atoi(const char*);
int strtol(const char*, char**, int);
unsigned int strtoul(const char*, char**, int);

// Random number functions
#define RAND_MAX INT_MAX
int rand();
void srand(unsigned int);

// Dynamic memory handling
void* malloc(size_t);
void* calloc(size_t, size_t);
void free(void*);
void* realloc(void*, size_t);

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// Environment
void abort() __attribute__((noreturn));
//int atexit(fp_t);
void exit(int) __attribute__((noreturn));

// Searching and ordering
typedef int (* c_comparator)(const void*, const void*);
void* bsearch(const void*, const void*, size_t, size_t, c_comparator);
void qsort(void*, size_t, size_t, c_comparator);

// Absolute value
#define labs abs
int abs(int);

// Division
#define ldiv div
#define ldiv_t div_t
typedef struct { int quot, rem; } div_t;
div_t div(int, int);

// These functions will never be implemented:
// atof, strtod, getenv, system, mblen, mbtowc, wctomb, mbstowcs, wcstombs

#ifdef __cplusplus
}
#endif
