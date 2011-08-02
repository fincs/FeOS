//
// FeOS C Standard Library
// stddef.h
//     Standard C definitions
//

#pragma once
#include <feosbase.h>

#define offsetof(type, member) __builtin_offsetof(type, member)

typedef long_t ptrdiff_t;
typedef word_t size_t;
typedef long_t ssize_t;

#ifndef __cplusplus
#define NULL ((void*)0)
#else
#define NULL 0
#endif
