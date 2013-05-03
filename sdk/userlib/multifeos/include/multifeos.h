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

/*! \file multifeos.h
	\brief FeOS user-mode cooperative multithreading library
*/

#define DEFAULT_STACK_SIZE (8*1024) //!< Default thread stack size.
// Ersatz future-proof binary priority system
#define PRIORITY_HIGH   0x7FFFFFFF //!< High priority.
#define PRIORITY_NORMAL 0 //!< Low priority

typedef void* thread_t; //!< Thread handle datatype.
typedef int (*threadEP_t)(void* param); //!< Thread entry point callback datatype.

#define FeOS_CancelThread FeOS_FreeThread //!< Alternative name for FeOS_FreeThread().

//! \brief Creates a thread.
MULTIFEOS_API thread_t FeOS_CreateThread(word_t stackSize, threadEP_t entryPoint, void* param);
//! \brief Yields thread execution. Normally an IRQ wait function should be used instead.
MULTIFEOS_API void FeOS_Yield();
//! \brief Gets the currently executing thread.
MULTIFEOS_API thread_t FeOS_GetCurrentThread();
//! \brief Exits the current thread.
MULTIFEOS_API void FeOS_ExitThread(int rc);
//! \brief Determines whether a certain thread has not finished.
MULTIFEOS_API bool FeOS_IsThreadActive(thread_t hThread);
//! \brief Frees/deletes a thread.
MULTIFEOS_API void FeOS_FreeThread(thread_t hThread);
//! \brief Sets the priority of a thread.
MULTIFEOS_API void FeOS_SetThreadPrio(thread_t hThread, int prio);
//! \brief Gets the exit code of a thread.
MULTIFEOS_API int FeOS_GetThreadRC(thread_t hThread);
//! \brief Waits for a thread to finish executing. The thread is additionally freed.
MULTIFEOS_API int FeOS_ThreadJoin();

//! \brief Asynchronously runs a command.
MULTIFEOS_API thread_t FeOS_CreateProcess(int argc, const char* argv[]);
//! \brief Asynchronously runs a shell command.
MULTIFEOS_API thread_t FeOS_RunAsync(const char* command);

//! \brief Makes the thread be automatically free when it exits.
MULTIFEOS_API void FeOS_DetachThread(thread_t hThread);
//! \brief Runs a piece of code in the context of another thread.
MULTIFEOS_API int FeOS_RunInContext(thread_t hThread, threadEP_t func, void* param);

//! \brief MultiFeOS-safe version of FeOS_NextIRQ().
MULTIFEOS_API word_t FeOS_NextIRQYield();

//! \brief MultiFeOS-safe way of idling.
static inline void FeOS_Idle()
{
	FeOS_WaitForIRQ(~0);
}

// Simple lock API

typedef volatile word_t lock_t; //!< Simple lock datatype

//! \brief Acquires a lock.
//! \param bIrqIdling Determines if IRQ-waiting is to be used as an idling method.
static inline void FeOS_AcquireLock(lock_t* pLock, bool bIrqIdling)
{
	while (*pLock) (bIrqIdling ? FeOS_Idle : FeOS_Yield)();
	*pLock = 1;
}

//! \brief Releases a lock.
static inline void FeOS_ReleaseLock(lock_t* pLock)
{
	*pLock = 0;
}

#ifdef __cplusplus
}
#endif
