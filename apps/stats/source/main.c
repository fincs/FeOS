#include <stdio.h>
#include <feos.h>

void printStats(const char* name, usagestats_t* pSt)
{
	printf("%s statistics:\n"
	       "\tFree:  %u bytes\n"
	       "\tUsed:  %u bytes\n"
	       "\tTotal: %u bytes\n\n", name, pSt->free, pSt->used, pSt->total);
}

void printStats64(const char* name, usagestats64_t* pSt)
{
	printf("%s statistics:\n"
	       "\tFree:  %llu bytes\n"
	       "\tUsed:  %llu bytes\n"
	       "\tTotal: %llu bytes\n\n", name, pSt->free, pSt->used, pSt->total);
}

int main()
{
	usagestats_t st;
	usagestats64_t st64;

	FeOS_GetMemStats(&st);
	printStats("Memory", &st);
	if (FeOS_GetDiskStats64(&st64))
		printStats64("Disk", &st64);
	return 0;
}
