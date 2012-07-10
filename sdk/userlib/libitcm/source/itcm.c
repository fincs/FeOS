#include <itcm.h>
#include "bget.h"

FEOSINIT void _init()
{
	bpool((void*)0x01001000, 28*1024);
}

FEOS_EXPORT void* itcm_malloc(size_t size)
{
	return bget(size);
}

FEOS_EXPORT void itcm_free(void* ptr)
{
	brel(ptr);
}
