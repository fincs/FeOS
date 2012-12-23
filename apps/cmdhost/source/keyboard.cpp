#include "common.h"

#define KEY_W 24
#define KEY_H 24

typedef struct
{
	int act, x, y;
} keydef_t;

#define DEF_KEY(c, sx, sy, i) { (c), (sx)+(i)*(KEY_W+1), (sy) }
#define KEY_COUNT 47

static const keydef_t keysNormal[]
{
	// Top row
	DEF_KEY('-', 9, 15, 0),
	DEF_KEY('=', 9, 15, 1),
	DEF_KEY('[', 9, 15, 2),
	DEF_KEY(']', 9, 15, 3),
	DEF_KEY('\\', 9, 15, 4),
	DEF_KEY('`', 9, 15, 5),
	DEF_KEY(';', 9, 15, 6),
	DEF_KEY('\'', 9, 15, 7),

	// Number row
	DEF_KEY('1', 3, 43, 0),
	DEF_KEY('2', 3, 43, 1),
	DEF_KEY('3', 3, 43, 2),
	DEF_KEY('4', 3, 43, 3),
	DEF_KEY('5', 3, 43, 4),
	DEF_KEY('6', 3, 43, 5),
	DEF_KEY('7', 3, 43, 6),
	DEF_KEY('8', 3, 43, 7),
	DEF_KEY('9', 3, 43, 8),
	DEF_KEY('0', 3, 43, 9),

	// First letter row
	DEF_KEY('q', 3, 68, 0),
	DEF_KEY('w', 3, 68, 1),
	DEF_KEY('e', 3, 68, 2),
	DEF_KEY('r', 3, 68, 3),
	DEF_KEY('t', 3, 68, 4),
	DEF_KEY('y', 3, 68, 5),
	DEF_KEY('u', 3, 68, 6),
	DEF_KEY('i', 3, 68, 7),
	DEF_KEY('o', 3, 68, 8),
	DEF_KEY('p', 3, 68, 9),

	// Second letter row
	DEF_KEY('a', 15, 93, 0),
	DEF_KEY('s', 15, 93, 1),
	DEF_KEY('d', 15, 93, 2),
	DEF_KEY('f', 15, 93, 3),
	DEF_KEY('g', 15, 93, 4),
	DEF_KEY('h', 15, 93, 5),
	DEF_KEY('j', 15, 93, 6),
	DEF_KEY('k', 15, 93, 7),
	DEF_KEY('l', 15, 93, 8),

	// Third letter row
	DEF_KEY('z', 3, 118, 0),
	DEF_KEY('x', 3, 118, 1),
	DEF_KEY('c', 3, 118, 2),
	DEF_KEY('v', 3, 118, 3),
	DEF_KEY('b', 3, 118, 4),
	DEF_KEY('n', 3, 118, 5),
	DEF_KEY('m', 3, 118, 6),
	DEF_KEY(',', 3, 118, 7),
	DEF_KEY('.', 3, 118, 8),
	DEF_KEY('/', 3, 118, 9),
};

static const keydef_t keysShifted[]
{
	// Top row
	DEF_KEY('_', 9, 15, 0),
	DEF_KEY('+', 9, 15, 1),
	DEF_KEY('{', 9, 15, 2),
	DEF_KEY('}', 9, 15, 3),
	DEF_KEY('|', 9, 15, 4),
	DEF_KEY('~', 9, 15, 5),
	DEF_KEY(':', 9, 15, 6),
	DEF_KEY('"', 9, 15, 7),

	// Number row
	DEF_KEY('!', 3, 43, 0),
	DEF_KEY('@', 3, 43, 1),
	DEF_KEY('#', 3, 43, 2),
	DEF_KEY('$', 3, 43, 3),
	DEF_KEY('%', 3, 43, 4),
	DEF_KEY('^', 3, 43, 5),
	DEF_KEY('&', 3, 43, 6),
	DEF_KEY('*', 3, 43, 7),
	DEF_KEY('(', 3, 43, 8),
	DEF_KEY(')', 3, 43, 9),

	// First letter row
	DEF_KEY('Q', 3, 68, 0),
	DEF_KEY('W', 3, 68, 1),
	DEF_KEY('E', 3, 68, 2),
	DEF_KEY('R', 3, 68, 3),
	DEF_KEY('T', 3, 68, 4),
	DEF_KEY('Y', 3, 68, 5),
	DEF_KEY('U', 3, 68, 6),
	DEF_KEY('I', 3, 68, 7),
	DEF_KEY('O', 3, 68, 8),
	DEF_KEY('P', 3, 68, 9),

	// Second letter row
	DEF_KEY('A', 15, 93, 0),
	DEF_KEY('S', 15, 93, 1),
	DEF_KEY('D', 15, 93, 2),
	DEF_KEY('F', 15, 93, 3),
	DEF_KEY('G', 15, 93, 4),
	DEF_KEY('H', 15, 93, 5),
	DEF_KEY('J', 15, 93, 6),
	DEF_KEY('K', 15, 93, 7),
	DEF_KEY('L', 15, 93, 8),

	// Third letter row
	DEF_KEY('Z', 3, 118, 0),
	DEF_KEY('X', 3, 118, 1),
	DEF_KEY('C', 3, 118, 2),
	DEF_KEY('V', 3, 118, 3),
	DEF_KEY('B', 3, 118, 4),
	DEF_KEY('N', 3, 118, 5),
	DEF_KEY('M', 3, 118, 6),
	DEF_KEY('<', 3, 118, 7),
	DEF_KEY('>', 3, 118, 8),
	DEF_KEY('?', 3, 118, 9),
};

typedef struct
{
	int c, x, y, w, h;
} specialkey_t;

#define SPECIAL_KEY_COUNT 5
#define KEY_SHIFT '\a'
#define KEY_CAPSLOCK '\f'

static const specialkey_t specialKeys[] =
{
	{ '\b', 220, 25, 32, 16 },
	{ '\n', 216, 144, 32, 16 },

	{ KEY_SHIFT, 4, 144, KEY_W, KEY_H },
	{ KEY_CAPSLOCK, 30, 144, KEY_W, KEY_H },

	{ ' ', 59, 155, 138, 26 },
};

static inline const keydef_t* getKeyMap(bool shifted)
{
	return shifted ? keysShifted : keysNormal;
}

CKeyboard::CKeyboard(CConsole& _con) : con(_con), bShifted(false), bCapsLock(false), readBuf(nullptr)
{
	font = g_guiManager->GetSystemFont();
}

#define INDICATOR_COLOR MakeColor(0,24,0)

#define SHIFT_IND_X 22
#define SHIFT_IND_Y 146
#define CPSLK_IND_X 48
#define CPSLK_IND_Y 146

static void drawIndicator(u16* bmpBuf, int x, int y)
{
	for (int j = 0; j < 4; j ++)
		for (int i = 0; i < 4; i ++)
			bmpBuf[x + i + 256*(y + j)] = INDICATOR_COLOR;
}

void CKeyboard::renderLabels(u16* bmpBuf)
{
	const keydef_t* keyMap = getKeyMap(bShifted);

	dmaFillHalfWords(Colors::Transparent, bmpBuf, 256*192*2);

	char b[2];
	b[1] = 0;
	for (int i = 0; i < KEY_COUNT; i ++)
	{
		b[0] = keyMap->act;

		font->PrintText(bmpBuf, keyMap->x+4, keyMap->y, b);
		keyMap ++;
	}

	if (bShifted ^ bCapsLock)
		drawIndicator(bmpBuf, SHIFT_IND_X, SHIFT_IND_Y);
	if (bCapsLock)
		drawIndicator(bmpBuf, CPSLK_IND_X, CPSLK_IND_Y);
}

ssize_t CKeyboard::handleRead(char* buf, size_t len)
{
	readBuf = buf;
	readBufPos = 0;
	readBufLen = len;
	while (readBuf)
		swiWaitForVBlank();
	return readBufPos;
}

int CKeyboard::getKey(CTouchPos& pos)
{
	const keydef_t* keyMap = getKeyMap(bShifted);
	const specialkey_t* spKeys = specialKeys;
	int key = -1;

	for (int i = 0; key < 0 && i < SPECIAL_KEY_COUNT; i ++, spKeys ++)
		if (pos.inRegion(spKeys->x, spKeys->y, spKeys->w, spKeys->h))
			key = spKeys->c;

	for (int i = 0; key < 0 && i < KEY_COUNT; i ++, keyMap ++)
		if (pos.inRegion(keyMap->x, keyMap->y, KEY_W, KEY_H))
			key = keyMap->act;

	return key;
}

bool CKeyboard::handleClick(u16* bmpBuf, CTouchPos& pos)
{
	int k = getKey(pos);
	if (k < 0)
		return false;

	if (k == KEY_SHIFT || k == KEY_CAPSLOCK)
	{
		bShifted = !bShifted;
		if (k == KEY_CAPSLOCK)
			bCapsLock = !bCapsLock;
		renderLabels(bmpBuf);
		return true;
	} else if (bShifted ^ bCapsLock)
	{
		bShifted = !bShifted;
		renderLabels(bmpBuf);
	}

	if (isReading())
	{
		if (k == '\b')
		{
			if (readBufPos == 0)
				return true;
			readBufPos --;
		} else
			readBuf[readBufPos++] = k;

		if (k == '\n' || readBufPos == (int)readBufLen)
			readBuf = nullptr;

		con.print(k);
	}

	return true;
}
