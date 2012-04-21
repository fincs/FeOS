#include <stdio.h>
#include <feos.h>

void printStats(const char* name, usagestats_t* pSt)
{
	printf("%s statistics:\n"
	       "\tFree:  %d bytes\n"
	       "\tUsed:  %d bytes\n"
	       "\tTotal: %d bytes\n\n", name, pSt->free, pSt->used, pSt->total);
}

int main()
{
	usagestats_t st;
	FeOS_GetMemStats(&st);
	printStats("Memory", &st);
	if (FeOS_GetDiskStats(&st))
		printStats("Disk", &st);
	return 0;
}
