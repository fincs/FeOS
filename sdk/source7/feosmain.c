#include <feos.h>

int arm7_main(int fifoCh);
void arm7_fini();

extern fp_t __feos_init_array[];
extern word_t __feos_init_size;

extern fp_t __feos_fini_array[];
extern word_t __feos_fini_size;

word_t __FeOSMain(word_t event, word_t prm1, word_t prm2, word_t prm3)
{
	register word_t i;

	switch(event)
	{
		case FEOS_EP_INIT:
		{
			for(i = 0; i < __feos_init_size; i ++)
				__feos_init_array[i]();
			return arm7_main(prm1) ? FEOS_RC_ERR : FEOS_RC_OK;
		}

		case FEOS_EP_FINI:
		{
			arm7_fini();
			for(i = 0; i < __feos_fini_size; i ++)
				__feos_fini_array[i]();
			return FEOS_RC_OK;
		}
	}

	return FEOS_RC_ERR;
}
