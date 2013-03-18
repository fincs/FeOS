#pragma once

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//! \brief Touch position wrapper class
//! \headerfile coopgui.h <coopgui.h>
class TouchPos : public touchPosition
{
public:
	//! \brief Constructor that automatically reads the stylus position.
	inline TouchPos()
	{
		touchRead(this);
	}

	//! \brief Constructor that receives the x and y positions.
	inline TouchPos(int x, int y)
	{
		px = x;
		py = y;
	}

	//! \brief Determines whether the position is inside the specified coordinate box.
	inline bool InRegion(int x, int y, int w, int h) const
	{
		int x2 = x + w;
		int y2 = y + h;
		return (px >= x && px < x2) && (py >= y && py < y2);
	}
};

/** @} */

}
}
