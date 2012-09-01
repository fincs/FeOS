#pragma once

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

struct IFont
{
	virtual void Dispose() = 0;

	virtual int GetHeight() = 0;
	virtual int GetBaseline() = 0;
	virtual bool LoadGlyph(int codePoint, glyph_t* /*out*/ pGlyph);

	// Print text in a 256x192 16-bit bitmap buffer
	virtual int PrintText(color_t* buf, int x, int y, const char* text, color_t brush = Colors::Black, word_t flags = 0) = 0;
};

class FontPtr
{
	IFont* ptr;

	inline void Clear()
	{
		if (ptr)
		{
			ptr->Dispose();
			ptr = nullptr;
		}
	}

	inline void Set(IFont* newPtr)
	{
		Clear();
		ptr = newPtr;
	}

public:

	inline FontPtr() : ptr(nullptr) { }
	inline FontPtr(IFont* newPtr) : ptr(newPtr) { }
	inline FontPtr(FontPtr& oPtr) : ptr(oPtr.ptr) { oPtr.ptr = nullptr; }
	inline ~FontPtr() { Clear(); }

	inline FontPtr& operator = (IFont* x)
	{
		Set(x);
		return *this;
	}

	inline FontPtr& operator = (FontPtr& x)
	{
		Set(x.ptr);
		x.ptr = nullptr;
		return *this;
	}

	inline IFont* operator -> () { return ptr; }
	inline operator IFont* () { return ptr; }
};

__COOPGUI_NAMESPACE_END
