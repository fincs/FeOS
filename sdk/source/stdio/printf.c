#include <stdio.h>

int printf(const char* fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	int rc = vprintf(fmt, v);
	va_end(v);
	return rc;
}
