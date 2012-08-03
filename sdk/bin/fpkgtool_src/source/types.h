#pragma once
#ifndef _MSC_VER
#include <sys/param.h>
#else
#define BIG_ENDIAN 1
#define LITTLE_ENDIAN 0
#define BYTE_ORDER LITTLE_ENDIAN
#endif

typedef unsigned int word_t;
typedef unsigned short hword_t;
typedef unsigned char byte_t;
typedef signed int long_t;
typedef signed short short_t;
typedef signed char char_t;

#define foreach(type,i,ctr) for(type i = (ctr).begin(); i != (ctr).end(); ++i)

#ifndef BYTE_ORDER
#error What's the endian of the platform you're targeting?
#endif

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

#if BYTE_ORDER == LITTLE_ENDIAN
#define be_word(a) eswap_word(a)
#define be_hword(a) eswap_hword(a)
#define le_word(a) (a)
#define le_hword(a) (a)
#elif BYTE_ORDER == BIG_ENDIAN
#define be_word(a) (a)
#define be_hword(a) (a)
#define le_word(a) eswap_word(a)
#define le_hword(a) eswap_hword(a)
#else
#error What's the endian of the platform you're targeting?
#endif
