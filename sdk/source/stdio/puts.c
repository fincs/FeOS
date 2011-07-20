#include <stdio.h>

int puts(const char* str)
{
	FILE* f = stdout;

	int rc = fputs(str, f);
	fputc('\n', f);
	return rc;
}
