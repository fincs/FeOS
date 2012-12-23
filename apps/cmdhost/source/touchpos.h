#pragma once
#include "common.h"

class CTouchPos : public touchPosition
{
public:
	inline CTouchPos()
	{
		touchRead(this);
	}
	inline bool inRegion(int x, int y, int w, int h)
	{
		int x2 = x + w;
		int y2 = y + h;
		return (px >= x && px < x2) && (py >= y && py < y2);
	}
};
