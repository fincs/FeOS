#pragma once

__COOPGUI_NAMESPACE_BEGIN

//-----------------------------------------------------------------------
// Generic video-related stuff
//-----------------------------------------------------------------------

typedef hword_t color_t;

static inline constexpr color_t MakeColor(int r, int g, int b)
{
	return ARGB16(1, r, g, b);
}

namespace Colors
{
	enum
	{
		Transparent = 0,
		Black   = MakeColor(0,0,0),
		White   = MakeColor(31,31,31),
		Red     = MakeColor(31,0,0),
		Green   = MakeColor(0,31,0),
		Blue    = MakeColor(0,0,31),
		Yellow  = MakeColor(31,31,0),
		Cyan    = MakeColor(0,31,31),
		Magenta = MakeColor(31,0,31)
	};
}

__COOPGUI_NAMESPACE_END
