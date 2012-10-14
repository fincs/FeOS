#include <stdlib.h>
#include <string.h>

char* strdup(const char* str)
{
	if (!str) return NULL;

	int size = strlen(str) + 1;
	char* newstr = (char*) malloc(size);
	if (!newstr) return NULL;

	memcpy(newstr, str, size);
	return newstr;
}
