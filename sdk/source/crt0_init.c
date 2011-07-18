#include <feos.h>

extern fp_t __feos_init_array[];
extern word_t __feos_init_size;

static void RunCtors()
{
	word_t i;
	for(i = 0; i < __feos_init_size; i ++)
		__feos_init_array[i]();
}

void crt0_init()
{
	RunCtors();
}
