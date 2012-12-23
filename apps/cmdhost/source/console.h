#pragma once
#include "common.h"

class CConsole
{
	u16 mapBuf[32*24];
	int savedX, savedY;
	int pal;

	char escapeSeq[8];
	int escapeLen;

	void newrow();

public:
	int x, y;

	inline CConsole() : pal(15 << 12), escapeLen(-1) { clear(); }

	inline void clear()
	{
		memset(mapBuf, 0, sizeof(mapBuf));
		x = y = 0;
	}

	void print(int c);
	int print(const char* str, int length = -1);

	inline void render(u16* pos)
	{
		DC_FlushRange(mapBuf, sizeof(mapBuf));
		dmaCopy(mapBuf, pos, sizeof(mapBuf));
	}
};
