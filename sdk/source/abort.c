#include <feos.h>
#include <stdlib.h>

void abort()
{
	FeOS_DebugPrint("abort() called!\n");
	exit(-1);
}
