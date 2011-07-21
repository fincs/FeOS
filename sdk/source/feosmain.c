#include <feos.h>
#include <stddef.h>
#include <setjmp.h>

word_t FEOS_WEAK FeOSMain(word_t, word_t, word_t, word_t);
int FEOS_WEAK main(int, const char*[]);

void crt0_init();
void crt0_fini();
void this_exit(int);

jmp_buf __exit_buf;
volatile int __exit_done;

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
			if (!main) return FEOS_RC_ERR;

			if (!FeOS_PushExitFunc(this_exit)) return FEOS_RC_ERR;

			__exit_done = 0;
			int rc = setjmp(__exit_buf);

			if (__exit_done) return (word_t) rc;

			word_t rc2 = (word_t) main((int) prm1, (const char**) prm2);
			FeOS_PopExitFunc();
			return rc2;

		default:
			if (FeOSMain) return FeOSMain(event, prm1, prm2, prm3);
			else return FEOS_RC_ERR;
	}
}
