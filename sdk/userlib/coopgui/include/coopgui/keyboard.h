#pragma once
#pragma interface

__COOPGUI_NAMESPACE_BEGIN

//-----------------------------------------------------------------------
// Keyboard API
//-----------------------------------------------------------------------

struct IKeyboard
{
	virtual void Dispose() = 0;
	virtual int GetInterfaceVersion() = 0;

	// Interface version 1 functions
	// -----------------------------

	// Set the 16-bit bitmap drawing buffer for key labels and indicators.
	virtual void SetDrawBuffer(color_t* bmpBuf) = 0;

	// Retrieve the graphics for the tiled background (8bpp text).
	virtual const GrfFile* GetGraphics() = 0;

	// This function is to be called when the stylus has just been pressed.
	// Returns the typed character (Unicode codepoint). Special values:
	// < 0: No key pressed
	// 0: Key pressed, but didn't generate a character
	virtual int OnClick(const TouchPos& pos) = 0;

	// Synchronous read - NOT meant to be used from the GUI thread
	virtual ssize_t SyncRead(char* buf, size_t len) = 0;
	virtual bool InSyncRead() = 0;
};

using KeyboardPtr = ResPtr<IKeyboard>;

__COOPGUI_NAMESPACE_END
