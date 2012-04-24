#pragma once
#include "types.h"

#define FAR_MAGIC 0x00524146 // 'FAR\0'

typedef struct
{
	word_t flags;
	word_t nameoff;
	word_t dataoff;
	word_t size;
} FARentry_t;

enum
{
	FAR_EFLAG_USRMASK = 0xFFFFFF00,
	FAR_EFLAG_TYPEMASK = 0xFF,
	
	FAR_ETYPE_FILE = 0,
	FAR_ETYPE_DIR = 1
};

#define FAR_EFLAG_GETUSR(x) ((x)>>8)
#define FAR_EFLAG_GETTYPE(x) ((x)&0xFF)

typedef struct
{
	word_t magic;
	word_t version;
	word_t nentries;
	word_t namesize;
	word_t topentries;
	//FARentry_t topdir[nentries];
} FARheader_t;
