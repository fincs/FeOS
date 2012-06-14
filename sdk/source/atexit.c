#include "atexit_intl.h"

int atexit(void (*callback)(void))
{
	return __reg_atexit((atexitCb)callback, NULL);
}
