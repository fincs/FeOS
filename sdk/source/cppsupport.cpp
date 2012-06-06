#include <feos.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" void __cxa_pure_virtual()
{
	fprintf(stderr, "{C++} Pure virtual call!\n");
	abort();
}
