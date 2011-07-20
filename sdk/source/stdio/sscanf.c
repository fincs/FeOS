#include <stdio.h>

int sscanf(const char* buf, const char* fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	int rc = vsscanf(buf, fmt, v);
	va_end(v);
	return rc;
}
