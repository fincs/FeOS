//
// MultiFeOS
// multifeos.h
//     FeOS user-mode cooperative multithreading library
//

#pragma once
#include <feos.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MULTIFEOS_BUILD
#define MULTIFEOS_API FEOS_EXPORT
#else
#define MULTIFEOS_API
#endif

#define DEFAULT_STACK_SIZE (8*1024)
// Ersatz future-proof binary priority system
#define PRIORITY_HIGH   0x7FFFFFFF
#define PRIORITY_NORMAL 0

typedef void* thread_t;
typedef int (*threadEP_t)(void* param);

#define FeOS_CancelThread FeOS_FreeThread

MULTIFEOS_API thread_t FeOS_CreateThread(word_t stackSize, threadEP_t entryPoint, void* param);
MULTIFEOS_API void FeOS_Yield();
MULTIFEOS_API thread_t FeOS_GetCurrentThread();
MULTIFEOS_API void FeOS_ExitThread(int rc);
MULTIFEOS_API bool FeOS_IsThreadActive(thread_t hThread);
MULTIFEOS_API void FeOS_FreeThread(thread_t hThread);
MULTIFEOS_API void FeOS_SetThreadPrio(thread_t hThread, int prio);
MULTIFEOS_API int FeOS_GetThreadRC(thread_t hThread);
MULTIFEOS_API int FeOS_ThreadJoin();

MULTIFEOS_API thread_t FeOS_CreateProcess(int argc, const char* argv[]);

MULTIFEOS_API word_t FeOS_NextIRQYield();

static inline void FeOS_Idle()
{
	FeOS_WaitForIRQ(~0);
}

// Simple lock API

typedef volatile word_t lock_t;

static inline void FeOS_AcquireLock(lock_t* pLock, bool bIrqIdling)
{
	while (*pLock) (bIrqIdling ? FeOS_Idle : FeOS_Yield)();
	*pLock = 1;
}

static inline void FeOS_ReleaseLock(lock_t* pLock)
{
	*pLock = 0;
}

#ifdef __cplusplus
}
#endif
