#include <feos.h>
#include <stddef.h>

word_t FEOS_WEAK FeOSMain(word_t, word_t, word_t, word_t);
int FEOS_WEAK main(int, const char*[]);

void crt0_init();
void crt0_fini();

word_t __FeOSMain(word_t event, word_t prm1, word_t prm2, word_t prm3)
{
	switch(event)
	{
		case FEOS_EP_INIT:
			crt0_init();
			return FEOS_RC_OK;

		case FEOS_EP_FINI:
			crt0_fini();
			return FEOS_RC_OK;

		case FEOS_EP_MAIN:
			if (main) return (word_t) main((int) prm1, (const char**) prm2);
			else return FEOS_RC_ERR;

		default:
			if (FeOSMain) return FeOSMain(event, prm1, prm2, prm3);
			else return FEOS_RC_ERR;
	}
}
