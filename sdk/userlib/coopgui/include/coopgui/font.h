#pragma once
#pragma interface

__COOPGUI_NAMESPACE_BEGIN

//-----------------------------------------------------------------------
// Font API
//-----------------------------------------------------------------------

typedef struct
{
	byte_t width, height;
	char_t posX, posY, advance;
	const byte_t* data;
} glyph_t;

__ENUM_BEGIN(PrintTextFlags, word_t)
	AtBaseline = BIT(0)
__ENUM_END(PrintTextFlags);

typedef struct
{
	color_t* buffer;
	int width, height, stride;
} surface_t;

struct IFont
{
	virtual void Dispose() = 0;

	virtual int GetHeight() = 0;
	virtual int GetBaseline() = 0;
	virtual bool LoadGlyph(int codePoint, glyph_t* /*out*/ pGlyph) = 0;

	// Print text in a 256x192 16-bit bitmap buffer
	virtual int PrintText(color_t* buf, int x, int y, const char* text, color_t brush = Colors::Black, word_t flags = 0) = 0;

	// Print text in a surface
	virtual int PrintText(const surface_t* s, int x, int y, const char* text, color_t brush = Colors::Black, word_t flags = 0) = 0;
};

using FontPtr = ResPtr<IFont>;

__COOPGUI_NAMESPACE_END
