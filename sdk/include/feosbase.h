//
// FeOS Standard Library
// feosbase.h
//     Base definitions for FeOS
//

#pragma once
#include <stdbool.h>

typedef unsigned char byte_t;
typedef unsigned short hword_t;
typedef unsigned int word_t;
typedef signed char char_t;
typedef signed short short_t;
typedef signed int long_t;
typedef void* instance_t;
typedef word_t file_t;
typedef word_t bool_t;
typedef long_t off_t;
typedef void (* fp_t)();

#define BIT(n) (1<<(n))

extern char __hinstance[];
#define FeOS_hInstance ((instance_t) __hinstance)

#ifdef __cplusplus
#define EXTERNC extern "C"
#define EXTERNC_DATA EXTERNC
#else
#define EXTERNC
#define EXTERNC_DATA extern
#endif

#define FEOS_EXPORT __attribute__((__visibility__("default")))
#define FEOS_WEAK __attribute__((weak))

#define FEOSENTRY EXTERNC
#define FEOSINIT __attribute__((constructor))
#define FEOSFINI __attribute__((destructor))

typedef void (* FeOSExitFunc)(int);
