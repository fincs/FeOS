#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void __sassert(const char* cond, const char* file, int line, const char* msg)
{
	fprintf(stderr, "Assertion failure!\n");
	fprintf(stderr, "Condition: %s\n", cond);
	fprintf(stderr, "%s:%d\n", file, line);
	if (msg) fprintf(stderr, "Message: %s\n", msg);
	abort();
}
