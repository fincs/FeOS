#include <stdio.h>

int vscanf(const char* fmt, va_list v)
{
	return vfscanf(stdin, fmt, v);
}
