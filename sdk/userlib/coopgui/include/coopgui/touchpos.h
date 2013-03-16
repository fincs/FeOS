#pragma once

__COOPGUI_NAMESPACE_BEGIN

// Touch position wrapper class
class TouchPos : public touchPosition
{
public:
	inline TouchPos()
	{
		touchRead(this);
	}

	inline TouchPos(int x, int y)
	{
		px = x;
		py = y;
	}

	inline bool InRegion(int x, int y, int w, int h) const
	{
		int x2 = x + w;
		int y2 = y + h;
		return (px >= x && px < x2) && (py >= y && py < y2);
	}
};

__COOPGUI_NAMESPACE_END
