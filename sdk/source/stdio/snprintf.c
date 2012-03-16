#include <stdio.h>

int snprintf(char* buf, size_t size, const char* fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	int rc = vsnprintf(buf, size, fmt, v);
	va_end(v);
	return rc;
}
