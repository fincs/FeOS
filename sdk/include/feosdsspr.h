//
// FeOS Standard Library
// feosdsspr.h
//     FeOS DS sprites
//

#pragma once

enum
{
	SpriteSize_8x8   = (0 << 14) | (0 << 12) | (8*8>>5),
	SpriteSize_16x16 = (1 << 14) | (0 << 12) | (16*16>>5),
	SpriteSize_32x32 = (2 << 14) | (0 << 12) | (32*32>>5),
	SpriteSize_64x64 = (3 << 14) | (0 << 12) | (64*64>>5),

	SpriteSize_16x8  = (0 << 14)  | (1 << 12) | (16*8>>5),
	SpriteSize_32x8  = (1 << 14) | (1 << 12) | (32*8>>5),
	SpriteSize_32x16 = (2 << 14) | (1 << 12) | (32*16>>5),
	SpriteSize_64x32 = (3 << 14) | (1 << 12) | (64*32>>5),

	SpriteSize_8x16  = (0 << 14)  | (2 << 12) | (8*16>>5),
	SpriteSize_8x32  = (1 << 14) | (2 << 12) | (8*32>>5),
	SpriteSize_16x32 = (2 << 14) | (2 << 12) | (16*32>>5),
	SpriteSize_32x64 = (3 << 14) | (2 << 12) | (32*64>>5)
};

enum
{
	SpriteMapping_1D_32      = (1 << 4) | (0 << 20) | (0 << 28) | 0,
	SpriteMapping_1D_64      = (1 << 4) | (1 << 20) | (1 << 28) | 1,
	SpriteMapping_1D_128     = (1 << 4) | (2 << 20) | (2 << 28) | 2,
	SpriteMapping_1D_256     = (1 << 4) | (3 << 20) | (3 << 28) | 3,
	SpriteMapping_2D         = (0 << 4) | (4 << 28),
	SpriteMapping_Bmp_1D_128 = (1 << 4) | (2 << 20) | (4 << 4) | (0 << 22) | (5 << 28) | 2,
	SpriteMapping_Bmp_1D_256 = (1 << 4) | (3 << 20) | (4 << 4) | (1 << 22) | (6 << 28) | 3,
	SpriteMapping_Bmp_2D_128 = (0 << 4) | (0 << 4) | (7 << 28) | 2,
	SpriteMapping_Bmp_2D_256 = (0 << 4) | (2 << 4) | (8 << 28) | 3
};

enum
{
	SpriteColorFormat_16Color  = 0,
	SpriteColorFormat_256Color = 1,
	SpriteColorFormat_Bmp      = 3
};

typedef struct
{
} OamState;

typedef struct
{
	hword_t attribute[3];
	hword_t filler;
} SpriteEntry;

#define oamMain (*_oamMain)
#define oamSub (*_oamSub)

extern OamState *_oamMain, *_oamSub;
extern SpriteEntry *oamMain_mem, *oamSub_mem;

OamState* FeOS_GetMainOAM();
OamState* FeOS_GetSubOAM();
SpriteEntry* FeOS_GetOAMMemory(OamState*);

void oamInit(OamState*, word_t, bool);
void oamDisable(OamState*);
void oamEnable(OamState*);
hword_t* oamGetGfxPtr(OamState*, int);
hword_t* oamAllocateGfx(OamState*, word_t, word_t);
void oamFreeGfx(OamState*, const void*);
void oamSetMosaic(word_t, word_t);
void oamSetMosaicSub(word_t, word_t);
void oamSet(OamState*, int, int, int, int, int, word_t, word_t, const void*, int, bool, bool, bool, bool, bool);
void oamClear(OamState*, int, int);
void oamClearSprite(OamState*, int);
void oamRotateScale(OamState*, int, int, int, int);
void oamAffineTransformation(OamState*, int, int, int, int, int);
