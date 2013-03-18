#pragma once

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//-----------------------------------------------------------------------
// Generic video-related stuff
//-----------------------------------------------------------------------

typedef hword_t color_t; //!< 16-bit color datatype

//! \brief Packs 5-bit RGB values into a 16-bit color
static inline constexpr color_t MakeColor(int r, int g, int b)
{
	return ARGB16(1, r, g, b);
}

/** @} */

namespace Colors
{
	/** @addtogroup coopgui Cooperative GUI
	 *  @{
	 */

	enum
	{
		Transparent = 0, //!< Transparent
		Black   = MakeColor(0,0,0), //!< Black
		White   = MakeColor(31,31,31), //!< White
		Red     = MakeColor(31,0,0), //!< Red
		Green   = MakeColor(0,31,0), //!< Green
		Blue    = MakeColor(0,0,31), //!< Blue
		Yellow  = MakeColor(31,31,0), //!< Yellow
		Cyan    = MakeColor(0,31,31), //!< Cyan
		Magenta = MakeColor(31,0,31) //!< Magenta
	};

	/** @} */
}

}
}
