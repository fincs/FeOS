#include <feos.h>
#include <stddef.h>
#include <string.h>
#include <setjmp.h>
#include <alloca.h>

word_t FEOS_WEAK FeOSMain(word_t, word_t, word_t, word_t);
int FEOS_WEAK main(int, const char*[]);

void crt0_init();
void crt0_fini();
void this_exit(int);

jmp_buf __exit_buf;
volatile int __exit_done;
volatile int __ninstances = 0;

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

		case FEOS_EP_GETEXIDXTBL:
		{
			typedef struct { word_t offset, size; } exidx_entry_t;
			extern exidx_entry_t* __exidx_start;
			extern exidx_entry_t* __exidx_end;

			FeOS_GetExidxTbl_t* out = (FeOS_GetExidxTbl_t*) prm1;
			out->table = &__exidx_start;
			out->nentries = &__exidx_end - &__exidx_start;
			return FEOS_RC_OK;
		}

		case FEOS_EP_MAIN:
		{
			word_t* bk_buf = NULL;

			if (!main) return FEOS_RC_ERR;

			if (__ninstances)
			{
				// Backup the exit jump buffer
				bk_buf = alloca(sizeof(jmp_buf));
				memcpy(bk_buf, __exit_buf, sizeof(jmp_buf));
			}

			if (!FeOS_PushExitFunc(this_exit)) return FEOS_RC_ERR;

			__ninstances ++;
			__exit_done = 0;
			int rc = setjmp(__exit_buf);

			if (__exit_done)
			{
				__ninstances --;
				if (bk_buf) memcpy(__exit_buf, bk_buf, sizeof(jmp_buf));
				return (word_t) rc;
			}

			word_t rc2 = (word_t) main((int) prm1, (const char**) prm2);
			FeOS_PopExitFunc();

			__ninstances --;
			if (bk_buf) memcpy(__exit_buf, bk_buf, sizeof(jmp_buf));
			return rc2;
		}

		default:
			if (FeOSMain) return FeOSMain(event, prm1, prm2, prm3);
			else return FEOS_RC_ERR;
	}
}
