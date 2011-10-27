//
// FeOS Standard Library
// feosdsbg.h
//     FeOS DS backgrounds
//

#pragma once

enum
{
	BgType_Text8bpp,
	BgType_Text4bpp,
	BgType_Rotation,
	BgType_ExRotation,
	BgType_Bmp8,
	BgType_Bmp16
};

enum
{
	BgSize_R_128x128   = (0 << 14),
	BgSize_R_256x256   = (1 << 14),
	BgSize_R_512x512   = (2 << 14),
	BgSize_R_1024x1024 = (3 << 14),

	BgSize_T_256x256 = (0 << 14) | (1 << 16),
	BgSize_T_512x256 = (1 << 14) | (1 << 16),
	BgSize_T_256x512 = (2 << 14) | (1 << 16),
	BgSize_T_512x512 = (3 << 14) | (1 << 16),

	BgSize_ER_128x128   = (0 << 14) | (2 << 16),
	BgSize_ER_256x256   = (1 << 14) | (2 << 16),
	BgSize_ER_512x512   = (2 << 14) | (2 << 16),
	BgSize_ER_1024x1024 = (3 << 14) | (2 << 16),

	BgSize_B8_128x128  =  ((0 << 14) | BIT(7) | (3 << 16)),
	BgSize_B8_256x256  =  ((1 << 14) | BIT(7) | (3 << 16)),
	BgSize_B8_512x256  =  ((2 << 14) | BIT(7) | (3 << 16)),
	BgSize_B8_512x512  =  ((3 << 14) | BIT(7) | (3 << 16)),
	BgSize_B8_1024x512 = (1 << 14) | (3 << 16),
	BgSize_B8_512x1024 = (0) | (3 << 16),

	BgSize_B16_128x128 = ((0 << 14) | BIT(7) | BIT(2) | (4 << 16)),
	BgSize_B16_256x256 = ((1 << 14) | BIT(7) | BIT(2) | (4 << 16)),
	BgSize_B16_512x256 = ((2 << 14) | BIT(7) | BIT(2) | (4 << 16)),
	BgSize_B16_512x512 = ((3 << 14) | BIT(7) | BIT(2) | (4 << 16)),
};

void bgSetRotate(int, int);
void bgRotate(int, int);
void bgSet(int, int, int, int, int, int, int, int);
void bgSetRotateScale(int, int, int, int);
void bgSetScale(int, int, int);
int bgInit(int, word_t, word_t, int, int);
int bgInitSub(int, word_t, word_t, int, int);
volatile hword_t* bgSetControlBits(int, hword_t);
void bgClearControlBits(int, hword_t);
void bgSetPriority(int, word_t);
void bgSetMapBase(int, word_t);
void bgSetTileBase(int, word_t);
void bgSetScrollf(int, int x, int y);
void bgMosaicEnable(int);
void bgMosaicDisable(int);
void bgSetMosaic(word_t dx, word_t dy);
void bgSetMosaicSub(word_t dx, word_t dy);
hword_t* bgGetMapPtr(int);
hword_t* bgGetGfxPtr(int);
int bgGetPriority(int);
int bgGetMapBase(int);
int bgGetTileBase(int);
void bgScrollf(int, int, int);
void bgShow(int);
void bgHide(int);
void bgSetCenterf(int, int, int);
void bgSetAffineMatrixScroll(int, int, int, int, int, int, int);

static inline void bgWrapOn(int id)
{
	bgSetControlBits(id, BIT(13));
}

static inline void bgWrapOff(int id)
{
	bgClearControlBits(id, BIT(13));
}

static inline void bgSetScroll(int id, int x, int y)
{
	bgSetScrollf(id, x << 8, y << 8);
}

static inline void bgScroll(int id, int dx, int dy)
{
	bgScrollf(id, dx << 8, dy << 8);
}

static inline void bgSetCenter(int id, int x, int y)
{
	bgSetCenterf(id, x << 8, y << 8);
}
