#include "atexit_intl.h"

void* __dso_handle;

int __aeabi_atexit(void* obj, atexitCb callback, void* dso_handle)
{
	return __reg_atexit(callback, obj);
}
