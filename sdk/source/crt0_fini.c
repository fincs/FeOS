#include <feos.h>

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
	RunDtors();
}
