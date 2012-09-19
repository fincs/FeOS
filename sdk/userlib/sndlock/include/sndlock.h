#pragma once
#include <feos.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int sndlock_t;

bool SndLock_Acquire(const char* name, sndlock_t* pLock);
bool SndLock_Release(sndlock_t lock);
const char* SndLock_GetName();

#ifdef __cplusplus
}
#endif
