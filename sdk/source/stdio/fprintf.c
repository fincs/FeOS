#include <stdio.h>

int fprintf(FILE* f, const char* fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	int rc = vfprintf(f, fmt, v);
	va_end(v);
	return rc;
}
