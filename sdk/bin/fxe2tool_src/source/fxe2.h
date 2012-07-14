#pragma once
#include "types.h"

typedef struct
{
	char magic[4]; // FZ01
	word_t loadsize;
	word_t bsssize;
	word_t nrelocs;

	byte_t osmajorver;
	byte_t osminorver;
	hword_t flags;
	word_t entrypoint;
	hword_t nexports;
	hword_t sexports;
	hword_t nimports;
	hword_t simports;
} fxe2_header_t;

typedef struct
{
	hword_t skip;
	hword_t patch;
} fxe2_reloc_t;

typedef struct
{
	word_t nameoffset;
	word_t address;
} fxe2_export_t;

typedef struct
{
	word_t nameoffset;
	word_t address;
} fxe2_import_t;

typedef struct
{
	word_t type;
	word_t count;
} fxe2_impcopy_head_t;

typedef struct
{
	word_t from;
	word_t to;
} fxe2_impcopy_t;

#define FXE2_MAGIC "FZ01"

enum
{
	FX2_M_SUBSYSTEM    = 0xF,
	FX2_SUBSYSTEM_INVALID  = 0,
	FX2_SUBSYSTEM_STANDARD = 1,

	FX2_M_LDRFLAGS          = 0xFFF0,
	FX2_LDRFLAGS_COMPRESSED = 0x0010,
	FX2_LDRFLAGS_HASIMPCOPY = 0x0020,

	FX2_IMP_SELECT_MODULE = 0xFFFFFFFF,

	FX2_IMPCOPY_NORMAL = 0
};

#define FX2_SUBSYSTEM(n) ((n) & FX2_M_SUBSYSTEM)
#define FX2_LDRFLAGS(n) ((n) & FX2_M_LDRFLAGS)
