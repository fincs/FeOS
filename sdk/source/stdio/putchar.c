#include <stdio.h>

int putchar(int ch)
{
	return fputc(ch, stdout);
}
