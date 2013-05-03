//
// FeOS Standard Library
// feosdsbg.h
//     FeOS libnds-like DS background API
//

#pragma once

/** @file feosdsbg.h
 *  \brief FeOS libnds-like DS background API
 */

/** @addtogroup api_dsbg FeOS libnds-like DS background API
 *  \brief FeOS libnds-like DS background API
 *  \code
 *  #include <feos.h>
 *  \endcode
 *  The DS contains two separate hardware 2D cores (A and B) responsible for rendering 2D backgrounds
 *  on each screen. This module outlines the API necessary to use this hardware functionality. The API
 *  provides basic initialization and management of the 8 2D backgrounds available on the DS. Other than
 *  initialization and hardware limitations, background control is identical on both engines. The following
 *  modes of operation are allowed:
 *  \code
Main 2D engine
______________________________
|Mode | BG0 | BG1 | BG2 |BG3 |           T = Text
|  0  |  T  |  T  |  T  |  T |           R = Rotation
|  1  |  T  |  T  |  T  |  R |           E = Extended Rotation
|  2  |  T  |  T  |  R  |  R |           L = Large Bitmap background
|  3  |  T  |  T  |  T  |  E |
|  4  |  T  |  T  |  R  |  E |
|  5  |  T  |  T  |  E  |  E |
|  6  |     |  L  |     |    |
-----------------------------

Sub 2D engine
______________________________
|Mode | BG0 | BG1 | BG2 |BG3 |
|  0  |  T  |  T  |  T  |  T |
|  1  |  T  |  T  |  T  |  R |
|  2  |  T  |  T  |  R  |  R |
|  3  |  T  |  T  |  T  |  E |
|  4  |  T  |  T  |  R  |  E |
|  5  |  T  |  T  |  E  |  E |
-----------------------------
 *  \endcode
 *  It is strongly recommended to use <a href="http://mtheall.com/vram.html">mtheall's VRAM BG allocation utility</a>
 *  in order to generate the necessary code for using backgrounds.
 *  @{
 */

//! \brief Background types
enum
{
	BgType_Text8bpp,
	BgType_Text4bpp,
	BgType_Rotation,
	BgType_ExRotation,
	BgType_Bmp8,
	BgType_Bmp16
};

//! \brief Background sizes
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

//! \brief Sets the rotation angle (counter-clockwise) of the specified background; and updates the hardware.
void bgSetRotate(int id, int angle);

//! \brief Cumulatively rotates the background counter-clockwise by the specified angle.
void bgRotate(int id, int angle);

//! \brief Sets all position, rotation and scaling properties of a background; and updates the hardware.
//! \param id Background ID.
//! \param angle Counter-clockwise angle, in DS angle units.
//! \param sx 8-bit fixed point horizontal scaling.
//! \param sy 8-bit fixed point vertical scaling.
//! \param scrollX 8-bit fixed point horizontal scroll.
//! \param scrollY 8-bit fixed point vertical scroll.
//! \param rotCenterX 8-bit fixed point rotation axis X coordinate.
//! \param rotCenterY 8-bit fixed point rotation axis Y coordinate.
void bgSet(int id, int angle, int sx, int sy, int scrollX, int scrollY, int rotCenterX, int rotCenterY);

//! \brief Sets the rotation and scaling properties of a background; and updates the hardware.
//! \param id Background ID.
//! \param angle Counter-clockwise angle, in DS angle units.
//! \param sx 8-bit fixed point horizontal scaling.
//! \param sy 8-bit fixed point vertical scaling.
void bgSetRotateScale(int id, int angle, int sx, int sy);

//! \brief Sets the scaling properties of a background; and updates the hardware.
//! \param id Background ID.
//! \param sx 8-bit fixed point horizontal scaling.
//! \param sy 8-bit fixed point vertical scaling.
void bgSetScale(int id, int sx, int sy);

//! \brief Initializes a background in engine A.
//! \param layer Background layer (0..3).
//! \param type Background type.
//! \param size Background size.
//! \param mapBase
//!    **Non-bitmap backgrounds**: 2K offset into background GFX RAM where the tilemap will be read from.<br>
//!    **Bitmap backgrounds**: 16K offset into background GFX RAM where the bitmap will be read from.
//! \param tileBase
//!    **Non-bitmap backgrounds**: 16K offset into background GFX RAM where the tile graphics will be read from.<br>
//!    **Bitmap backgrounds**: this parameter is unused and should be zero.
//! \returns The background ID.
int bgInit(int layer, word_t type, word_t size, int mapBase, int tileBase);

//! \brief Initializes a background in engine B.
//! \param layer Background layer (0..3).
//! \param type Background type.
//! \param size Background size.
//! \param mapBase
//!    **Non-bitmap backgrounds**: 2K offset into background GFX RAM where the tilemap will be read from.<br>
//!    **Bitmap backgrounds**: 16K offset into background GFX RAM where the bitmap will be read from.
//! \param tileBase
//!    **Non-bitmap backgrounds**: 16K offset into background GFX RAM where the tile graphics will be read from.<br>
//!    **Bitmap backgrounds**: this parameter is unused and should be zero.
//! \returns The background ID.
int bgInitSub(int layer, word_t type, word_t size, int mapBase, int tileBase);

//! \brief Updates the background hardware. Has no effect if AUTOUPD_BG is turned on.
void bgUpdate();

//! \cond
volatile hword_t* bgSetControlBits(int id, hword_t);
void bgClearControlBits(int id, hword_t);
//! \endcond

//! \brief Sets the priority of a background (0..3), a lower value means a higher priority.
void bgSetPriority(int id, word_t prio);
//! \brief Sets the map base of a background (see bgInit() or bgInitSub() for more details).
void bgSetMapBase(int id, word_t);
//! \brief Sets the tile base of a background (see bgInit() or bgInitSub() for more details).
void bgSetTileBase(int id, word_t);
//! \brief Sets the scrolling of a background (8-bit fixed point).
void bgSetScrollf(int id, int x, int y);
//! \brief Enables mosaic for a background.
void bgMosaicEnable(int id);
//! \brief Disables mosaic for a background.
void bgMosaicDisable(int id);
//! \brief Configures engine A background mosaic (0..15).
void bgSetMosaic(word_t dx, word_t dy);
//! \brief Configures engine B background mosaic (0..15).
void bgSetMosaicSub(word_t dx, word_t dy);
//! \brief Gets the pointer to a background's tilemap data.
hword_t* bgGetMapPtr(int id);
//! \brief Gets the pointer to a background's tile/bitmap gfx data.
hword_t* bgGetGfxPtr(int id);
//! \brief Gets the priority of a background (0..3).
int bgGetPriority(int id);
//! \brief Gets the map base of a background (see bgInit() or bgInitSub() for more details).
int bgGetMapBase(int id);
//! \brief Gets the tile base of a background (see bgInit() or bgInitSub() for more details).
int bgGetTileBase(int id);
//! \brief Accumulatively scrolls a background (8-bit fixed point).
void bgScrollf(int id, int x, int y);
//! \brief Shows a background.
void bgShow(int id);
//! \brief Hides a background.
void bgHide(int id);
//! \brief Sets the rotation center for a background (8-bit fixed point).
void bgSetCenterf(int id, int x, int y);
//! \brief Sets the raw affine transformation matrix of a background, and the scroll (8-bit fixed point).
void bgSetAffineMatrixScroll(int id, int hdx, int hdy, int vdx, int vdy, int scrollX, int scrollY);
//! \brief Enables engine A extended palettes.
void bgExtPaletteEnable();
//! \brief Enables engine B extended palettes.
void bgExtPaletteEnableSub();
//! \brief Disables engine A extended palettes.
void bgExtPaletteDisable();
//! \brief Disables engine A extended palettes.
void bgExtPaletteDisableSub();

//! \brief Enables wraparound in non-text background (text backgrounds are always wrapped).
static inline void bgWrapOn(int id)
{
	bgSetControlBits(id, BIT(13));
}

//! \brief Disables wraparound in non-text background (text backgrounds are always wrapped).
static inline void bgWrapOff(int id)
{
	bgClearControlBits(id, BIT(13));
}

//! \brief Sets the scrolling of a background (integer version).
static inline void bgSetScroll(int id, int x, int y)
{
	bgSetScrollf(id, x << 8, y << 8);
}

//! \brief Accumulatively scrolls a background (integer version).
static inline void bgScroll(int id, int dx, int dy)
{
	bgScrollf(id, dx << 8, dy << 8);
}

//! \brief Sets the rotation center for a background.
static inline void bgSetCenter(int id, int x, int y)
{
	bgSetCenterf(id, x << 8, y << 8);
}

/** @} */
