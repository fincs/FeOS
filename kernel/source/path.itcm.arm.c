#include <stdio.h>
#include <string.h>
#include "feos.h"
#include <limits.h>

// -1=no, 0=yes albeit read-only, 1=yes
int FeOS_IsValidName(const char* path)
{
	if(!AddressCheckMainRAM(path))
		return -1;

	char strbuf[PATH_MAX];
	register int i;
	for(i = 0;; i ++)
	{
		int c = path[i];
		strbuf[i] = (char) c;

		if(!path[i])
			break;

		if(i == PATH_MAX)
			return -1;

		if(c == '\\' || c == '.' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|')
			return -1;

		if(i == 0 && c != '/')
			return -1;
	}

	if(strnicmp(strbuf, "/sys", 5) == 0)
		return 0;
	else if(strnicmp(strbuf, "/filesys/FeOS", 13) == 0)
		return -1;

	return 1;
}
