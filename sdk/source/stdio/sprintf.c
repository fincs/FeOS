#include <stdio.h>

int sprintf(char* buf, const char* fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	int rc = vsprintf(buf, fmt, v);
	va_end(v);
	return rc;
}
