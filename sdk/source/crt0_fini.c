#include <feos.h>
#include "atexit_intl.h"

extern fp_t __feos_fini_array[];
extern word_t __feos_fini_size;

static void RunDtors()
{
	word_t i;
	for(i = 0; i < __feos_fini_size; i ++)
		__feos_fini_array[i]();
}

void crt0_fini()
{
	if (__call_atexit) __call_atexit();
	RunDtors();
}
