#include "atexit_intl.h"

void* __dso_handle;

int __aeabi_atexit(atexitCb callback, void* obj, void* dso_handle)
{
	return __reg_atexit(callback, obj);
}
