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

MULTIFEOS_API thread_t FeOS_CreateThread(word_t stackSize, threadEP_t entryPoint, void* param);
MULTIFEOS_API void FeOS_Yield();
MULTIFEOS_API thread_t FeOS_GetCurrentThread();
MULTIFEOS_API void FeOS_ExitThread(int rc);
MULTIFEOS_API bool FeOS_IsThreadActive(thread_t hThread);
MULTIFEOS_API void FeOS_FreeThread(thread_t hThread);
MULTIFEOS_API void FeOS_SetThreadPrio(thread_t hThread, int prio);
MULTIFEOS_API int FeOS_GetThreadRC(thread_t hThread);

MULTIFEOS_API thread_t FeOS_CreateProcess(int argc, const char* argv[]);

MULTIFEOS_API void FeOS_IRQWaitYield(word_t mask);
MULTIFEOS_API word_t FeOS_NextIRQYield();
MULTIFEOS_API void FeOS_IRQWaitCheck();

MULTIFEOS_API void FeOS_InstallYieldIRQ();
MULTIFEOS_API void FeOS_UninstallYieldIRQ();

#define FeOS_ThreadJoin FeOS_ThreadWaitClose

static inline int FeOS_ThreadWaitClose(thread_t hThread)
{
	int rc;
	while (FeOS_IsThreadActive(hThread)) FeOS_Yield();
	rc = FeOS_GetThreadRC(hThread);
	FeOS_FreeThread(hThread);
	return rc;
}

#ifdef __cplusplus
}
#endif
