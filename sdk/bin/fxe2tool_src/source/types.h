#pragma once
#ifndef _MSC_VER
#include <sys/param.h>
#else
#define BIG_ENDIAN 1
#define LITTLE_ENDIAN 0
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#include "../../../include/feosver.h"

typedef unsigned int word_t;
typedef unsigned short hword_t;
typedef unsigned char byte_t;
typedef signed int long_t;
typedef signed short short_t;
typedef signed char char_t;

#ifndef BYTE_ORDER
#error What's the endian of the platform you're targeting?
#endif

#if BYTE_ORDER == BIG_ENDIAN
#define BYTE_SHR_SHL(a, b, c) ((((a) >> (b)) & 0xFF) << (c))

static inline word_t eswap_word(word_t a)
{
	return BYTE_SHR_SHL(a, 0, 24)
	     | BYTE_SHR_SHL(a, 8, 16)
	     | BYTE_SHR_SHL(a, 16, 8)
	     | BYTE_SHR_SHL(a, 24, 0);
}

static inline hword_t eswap_hword(hword_t a)
{
	return BYTE_SHR_SHL(a, 0, 8) | BYTE_SHR_SHL(a, 8, 0);
}

#undef BYTE_SHR_SHL
#elif BYTE_ORDER == LITTLE_ENDIAN
#define eswap_word(a) (a)
#define eswap_hword(a) (a)
#else
#error What's the endian of the platform you're targeting?
#endif
