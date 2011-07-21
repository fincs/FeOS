#include <string.h>

size_t strxfrm(char* buf, const char* src, size_t bufsiz)
{
	size_t srclen = strlen(src);
	if (buf) strncpy(buf, src, bufsiz);
	return srclen;
}
