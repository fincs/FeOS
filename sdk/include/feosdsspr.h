//
// FeOS Standard Library
// feosdsspr.h
//     FeOS DS sprites
//

#pragma once

/** @file feosdsspr.h
 *  \brief FeOS libnds-like DS sprite API
 */

/** @addtogroup api_dsspr FeOS libnds-like DS sprite API
 *  \brief FeOS libnds-like DS sprite API
 *  \code
 *  #include <feos.h>
 *  \endcode
 *  @{
 */

//! \brief Sprite sizes.
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

//! \brief Sprite VRAM mapping modes.
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

//! \brief Sprite color formats.
enum
{
	SpriteColorFormat_16Color  = 0,
	SpriteColorFormat_256Color = 1,
	SpriteColorFormat_Bmp      = 3
};

//! \brief Sprite OAM state structure.
//! \headerfile feos.h <feos.h>
//!
//! This is an opaque datatype that contains no public fields.
typedef struct
{
} OamState;

//! \brief Sprite OAM entry structure.
//! \headerfile feos.h <feos.h>
typedef union
{
	struct
	{
		struct
		{
			u16 y :8; //!< Sprite Y position.
			union
			{
				struct
				{
					u8            :1;
					bool isHidden :1; //!< Sprite is hidden (isRotoscale cleared).
					u8            :6;
				};
				struct
				{
					bool isRotateScale :1; //!< Sprite uses affine parameters if set.
					bool isSizeDouble  :1; //!< Sprite bounds is doubled (isRotoscale set).
					u8 blendMode       :2; //!< Sprite object mode.
					bool isMosaic      :1; //!< Enables mosaic effect if set.
					u8 colorMode       :1; //!< Sprite color mode.
					u8 shape           :2; //!< Sprite shape.
				};
			};
		};

		union
		{
			struct
			{
				u16 x :9; //!< Sprite X position.
				u8    :7;
			};
			struct
			{
				u8 :8;
				union
				{
					struct
					{
						u8         :4;
						bool hFlip :1; //!< Flip sprite horizontally (isRotoscale cleared).
						bool vFlip :1; //!< Flip sprite vertically (isRotoscale cleared).
						u8         :2;
					};
					struct
					{
						u8               :1;
						u8 rotationIndex :5; //!< Affine parameter number to use (isRotoscale set).
						u8 size          :2; //!< Sprite size.
					};
				};
			};
		};

		struct
		{
			u16 gfxIndex :10; //!< Upper-left tile index.
			u8 priority  :2;  //!< Sprite priority.
			u8 palette   :4;  //!< Sprite palette to use in paletted color modes.
		};

		u16 attribute3; //!< Rotset filler
	};

	struct
	{
		u16 attribute[3];
		u16 filler;
	};

} SpriteEntry;

#define oamMain (*_oamMain) //!< Engine A OAM state. Pass `&oamMain` as the first parameter to sprite functions.
#define oamSub (*_oamSub) //!< Engine B OAM state. Pass `&oamSub` as the first parameter to sprite functions.

//! \cond
extern OamState *_oamMain, *_oamSub;
//! \endcond

//! \brief Engine A OAM entry table (128 entries)
extern SpriteEntry* oamMain_mem;
//! \brief Engine B OAM entry table (128 entries)
extern SpriteEntry* oamSub_mem;

//! \cond
OamState* DSGetMainOAM();
OamState* DSGetSubOAM();
SpriteEntry* DSGetOAMMemory(OamState*);
//! \endcond

//! \brief Initializes the 2D sprite engine.
//!
//! In order to mix tiled and bitmap sprites use SpriteMapping_Bmp_1D_128 or SpriteMapping_Bmp_1D_256.
void oamInit(OamState*, word_t sprMapping, bool useExtPalette);

//! \brief Updates the hardware's OAM memory. Has no effect if AUTOUPD_OAM is enabled.
void oamUpdate(OamState*);

//! \brief Disables sprite rendering.
void oamDisable(OamState*);
//! \brief Enables sprite rendering.
void oamEnable(OamState*);

//! \brief Translates a graphic offset index into a VRAM address
hword_t* oamGetGfxPtr(OamState*, int gfxOffsetIndex);

//! \brief Allocates graphics memory for the supplied sprite attributes.
hword_t* oamAllocateGfx(OamState*, word_t sprSize, word_t sprColorFormat);

//! \brief Frees memory obtained with oamAllocateGfx().
void oamFreeGfx(OamState*, const void* mem);

//! \brief Configures engine A sprite mosaic (0..15).
void oamSetMosaic(word_t dx, word_t dy);

//! \brief Configures engine B sprite mosaic (0..15).
void oamSetMosaicSub(word_t dx, word_t dy);

//! \brief Updates a sprite.
//! \param sprId Sprite ID (0..127).
//! \param x X coordinate.
//! \param y Y coordinate.
//! \param priority Priority (0..3).
//! \param palIdOrAlpha If 4-bit or 8-bit color sprite, this is the palette ID; if 16-bit bitmap sprite then it is alpha (0 stands for invisible). (0..15).
//! \param sprSize Sprite size.
//! \param sprFormat Sprite color format.
//! \param gfx Pointer to graphics in VRAM.
//! \param rotsetId Rotset ID to use (0..31), specify -1 to disable.
//! \param doubleSize Specifies whether the sprite frame will be 2x expanded (for rotation).
//! \param hide Specifies whether the sprite is hidden,
//! \param hflip Specifies whether to horizontally flip the sprite.
//! \param vflip Specifies whether to vertically flip the sprite.
//! \param mosaic Specifies whether mosaic is to be applied to the sprite.
void oamSet(OamState*, int sprId, int x, int y, int priority, int palIdOrAlpha, word_t sprSize, word_t sprFormat
	, const void* gfx, int rotsetId, bool doubleSize, bool hide, bool hflip, bool vflip, bool mosaic);

//! \brief Deletes sprites in the specified range.
void oamClear(OamState*, int startId, int count);

//! \brief Deletes a single sprite.
void oamClearSprite(OamState*, int sprId);

//! \brief Configures a rotset.
//! \param rotsetId Rotset ID (0..13).
//! \param angle Counter-clockwise angle, in DS angle units.
//! \param sx Inverse X axis scale factor, in 8-bit fixed point.
//! \param sy Inverse Y axis scale factor, in 8-bit fixed point.
void oamRotateScale(OamState*, int rotsetId, int angle, int sx, int sy);

//! \brief Configures a rotset using a raw 8-bit fixed point affine matrix.
void oamAffineTransformation(OamState*, int rotsetId, int hdx, int hdy, int vdx, int vdy);

//! \brief Converts a VRAM sprite graphic pointer to an OAM graphic offset.
unsigned int oamGfxPtrToOffset(OamState*, const void* gfx);

/** @} */
