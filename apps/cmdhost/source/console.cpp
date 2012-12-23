#include "common.h"

void CConsole::newrow()
{
	if (++y == 24)
	{
		y = 23;
		memcpy(mapBuf, mapBuf+32, 32*23*2);
		memset(mapBuf+23*32, 0, 32*2);
	}
}

void CConsole::print(int c)
{
	if (!c) return;

	switch (c)
	{
		case '\b':
		{
			x--;
			if (x == -1)
			{
				x = 31;
				y --;
				if (y == -1)
					y = 0;
			}
			mapBuf[x + y*32] = (int)' ' | pal;
			break;
		}
		case '\t':
		{
			x += 4 - (x & 3);
			break;
		}
		case '\n':
			newrow();
		case '\f':
			x = 0;
			break;
		default:
		{
			mapBuf[x + y*32] = (c & 0xFF) | pal;
			x ++;
			break;
		}
	}

	if (x == 32)
	{
		x = 0;
		newrow();
	}
}

int CConsole::print(const char* str, int length)
{
	if (length < 0)
		length = strlen(str);
	if (length == 0)
		return 0;

	const char* strEnd = str + length;
	while (str < strEnd)
	{
		int c = *str++;

		if (escapeLen >= 0)
		{
			if (escapeLen == 0 && c != '[')
			{
				print(0x1b);
				escapeLen = -1;
				str --;
				continue;
			}

			if (escapeLen == sizeof(escapeSeq))
			{
				for (int i = 0; i < (int)sizeof(escapeSeq); i ++)
					print(escapeSeq[i]);
				escapeLen = -1;
				str --;
				continue;
			}

			escapeSeq[escapeLen++] = c;

			bool bEscaping = false;
			int param;
			switch (c)
			{
				case 'A': // Move cursor up
					sscanf(escapeSeq, "[%dA", &param);
					y -= param;
					if (y < 0) y = 0;
					break;
				case 'B': // Move cursor down
					sscanf(escapeSeq, "[%dB", &param);
					y += param;
					if (y > 23) y = 23;
					break;
				case 'C': // Move cursor left
					sscanf(escapeSeq, "[%dC", &param);
					x += param;
					if (x > 31) x = 31;
					break;
				case 'D': // Move cursor right
					sscanf(escapeSeq, "[%dD", &param);
					x -= param;
					if (x < 0) x = 0;
					break;

				case 'H': // Move cursor to certain position
				case 'f':
					sscanf(escapeSeq, "[%d;%df", &y, &x);
					break;

				case 'J': // Clear screen (TODO: modes) (0: from pos to end, 1: from start to pos-1, 2: everything and clear pos too)
					clear();
					break;
				case 'K': // Clear line (TODO: modes) (0: from pos to end, 1: from start to pos-1, 2: everything)
					memset(mapBuf + y*32, 0, 32);
					break;

				case 's': // Save cursor
					savedX = x;
					savedY = y;
					break;
				case 'u': // Restore cursor
					x = savedX;
					y = savedY;
					break;

				case 'm': // Color
				{
					int intensity = 0;
					sscanf(escapeSeq, "[%d;%dm", &param, &intensity);

					// Only handle 30-37,39 and intensity for the color changes
					param -= 30;

					if (param == 9) param = 15; // Reset
					else if (param > 8) param -= 2;
					else if (intensity) param += 8;

					pal = (param & 0xF) << 12;
					break;
				}
				default:
					bEscaping = true;
					break;
			}

			if (!bEscaping)
				escapeLen = -1;
		} else if (c == 0x1b)
		{
			memset(escapeSeq, 0, sizeof(escapeSeq));
			escapeLen = 0;
		} else
			print(c);
	}

	return length;
}