#include <stdio.h>

char* gets(char* buf)
{
	return fgets(buf, 1024, stdin);
}
