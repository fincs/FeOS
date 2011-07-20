#include <stdio.h>

int vprintf(const char* fmt, va_list v)
{
	return vfprintf(stdout, fmt, v);
}
