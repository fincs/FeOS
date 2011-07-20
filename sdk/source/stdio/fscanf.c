#include <stdio.h>

int fscanf(FILE* f, const char* fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	int rc = vfscanf(f, fmt, v);
	va_end(v);
	return rc;
}
