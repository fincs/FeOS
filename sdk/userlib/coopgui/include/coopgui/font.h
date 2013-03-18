#pragma once
#pragma interface

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//-----------------------------------------------------------------------
// Font API
//-----------------------------------------------------------------------

//! \brief Font glyph data structure.
//! \headerfile coopgui.h <coopgui.h>
typedef struct
{
	byte_t width, height;
	char_t posX, posY, advance;
	const byte_t* data; //!< 1bpp data.
} glyph_t;

#ifndef DOXYGEN
__ENUM_BEGIN(PrintTextFlags, word_t)
	AtBaseline = BIT(0)
__ENUM_END(PrintTextFlags);
#endif

//! \brief 16-bit drawing surface data structure.
//! \headerfile coopgui.h <coopgui.h>
typedef struct
{
	//! 16-bit buffer.
	color_t* buffer;
	int width, height, stride;
} surface_t;

//! \brief Font interface.
//! \headerfile coopgui.h <coopgui.h>
struct IFont
{
	virtual void Dispose() = 0; //!< Releases the object.

	virtual int GetHeight() = 0; //!< Retrieves the font height.
	virtual int GetBaseline() = 0; //!< Retrieves the font baseline.
	virtual bool LoadGlyph(int codePoint, glyph_t* /*out*/ pGlyph) = 0; //!< \brief Loads a glyph.

	//! \brief Prints text in a 256x192 16-bit bitmap buffer.
	//! \param flags Printing flags.
	//!     - `PrintTextFlags::AtBaseline` -- The coordinates specify the baseline instead of the top left corner.
	virtual int PrintText(color_t* buf, int x, int y, const char* text, color_t brush = Colors::Black, word_t flags = 0) = 0;

	//! \brief Prints text in a surface.
	virtual int PrintText(const surface_t* s, int x, int y, const char* text, color_t brush = Colors::Black, word_t flags = 0) = 0;
};

//! \brief Font smart pointer datatype.
using FontPtr = ResPtr<IFont>;

/** @} */

}
}
