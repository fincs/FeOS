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
// Keyboard API
//-----------------------------------------------------------------------

//! \brief Keyboard interface.
//! \headerfile coopgui.h <coopgui.h>
struct IKeyboard
{
	virtual void Dispose() = 0; //!< Releases the object.
	virtual int GetInterfaceVersion() = 0; //!< Retrieves the interface version.

	// Interface version 1 functions
	// -----------------------------

	//! \brief Set the 16-bit bitmap drawing buffer for key labels and indicators.
	virtual void SetDrawBuffer(color_t* bmpBuf) = 0;

	//! \brief Retrieve the graphics for the tiled background (8bpp text).
	virtual const GrfFile* GetGraphics() = 0;

	//! \brief This function is to be called when the stylus has just been pressed.
	//! \returns The typed character (Unicode codepoint). Special values:
	//!   - < 0: No key pressed
	//!   - 0: Key pressed, but didn't generate a character
	virtual int OnClick(const TouchPos& pos) = 0;

	//! \brief Synchronous read -- NOT meant to be used from the GUI thread
	virtual ssize_t SyncRead(char* buf, size_t len) = 0;

	//! \brief Determines whether a synchronous read is taking place.
	virtual bool InSyncRead() = 0;
};

//! \brief Keyboard smart pointer datatype.
using KeyboardPtr = ResPtr<IKeyboard>;

/** @} */

}
}
