#pragma once
#include "common.h"

class CKeyboard
{
	CConsole& con;
	bool bShifted, bCapsLock;
	FontPtr font;
	char* readBuf;
	ssize_t readBufPos;
	size_t readBufLen;

	int getKey(CTouchPos& pos);

public:
	CKeyboard(CConsole& _con);
	void renderLabels(u16* bmpBuf);
	bool handleClick(u16* bmpBuf, CTouchPos& pos);
	ssize_t handleRead(char* buf, size_t len);

	inline bool isReading() { return readBuf != nullptr; }
};
