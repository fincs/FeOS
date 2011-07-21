#include <locale.h>
#include <stddef.h>

char* setlocale(int what, const char* loc)
{
	if (loc) return NULL; // failed to set locale
	return (char*)"C";
}
