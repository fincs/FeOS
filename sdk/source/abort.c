#include <feos.h>
#include <stdio.h>
#include <stdlib.h>

void abort()
{
	fprintf(stderr, "abort() called!\n");
	exit(-1);
}
