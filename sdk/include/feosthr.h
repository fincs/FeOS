//
// FeOS Standard Library
// feosthr.h
//     FeOS Threading API
//

#pragma once
#include <feos.h>

/** @file feosthr.h
 *  \brief FeOS Threading API
 */

/** @addtogroup api_thr FeOS Threading API
 *  @{
 */

#define DEFAULT_STACK_SIZE (8*1024) //!< Default thread stack size.
// Ersatz future-proof binary priority system
#define PRIORITY_HIGH   0x7FFFFFFF //!< High priority.
#define PRIORITY_NORMAL 0 //!< Low priority

typedef void* thread_t; //!< Thread handle datatype.
typedef int (*threadEP_t)(void* param); //!< Thread entry point callback datatype.

//! \brief Creates a thread.
thread_t ThrCreate(word_t stackSize, threadEP_t entryPoint, void* param);
//! \brief Yields thread execution. Normally an IRQ wait function should be used instead.
void ThrYield();
//! \brief Gets the currently executing thread.
thread_t ThrGetSelf();
//! \brief Exits the current thread.
void ThrExit(int rc);
//! \brief Determines whether a certain thread has not finished.
bool ThrIsActive(thread_t hThread);
//! \brief Frees/deletes a thread.
void ThrFree(thread_t hThread);
//! \brief Sets the priority of a thread.
void ThrSetPriority(thread_t hThread, int prio);
//! \brief Gets the exit code of a thread.
int ThrGetExitCode(thread_t hThread);
//! \brief Waits for a thread to finish executing. The thread is additionally freed.
int ThrJoin();

//! \brief Makes the thread be automatically free when it exits.
void ThrDetach(thread_t hThread);
//! \brief Runs a piece of code in the context of another thread.
int ThrRunInContext(thread_t hThread, threadEP_t func, void* param);

//! \brief Recommended way to idle.
static inline void ThrIdle()
{
	DSWaitForIRQ(~0);
}

//! \brief Creates a process using an argv array.
thread_t PsCreateFromArgv(int argc, const char* argv[]);
//! \brief Creates a process using a command line.
thread_t PsCreateFromCmdLine(const char* command);

//! \brief Allocates a thread-local storage slot.
//! \returns The index of the slot. If the function fails it returns a negative number.
int ThrTlsAlloc();
//! \brief Frees a thread-local storage slot.
void ThrTlsFree(int id);
//! \brief Retrieves the value in the current thread's specified thread-local storage slot.
void* ThrTlsGetValue(int id);
//! \brief Sets the value of the current thread's specified thread-local storage slot.
void ThrTlsSetValue(int id, void* value);

// Simple lock API

typedef volatile word_t lock_t; //!< Simple lock datatype

//! \brief Acquires a lock.
//! \param bIrqIdling Determines if IRQ-waiting is to be used as an idling method.
static inline void ThrAcquireLock(lock_t* pLock, bool bIrqIdling)
{
	while (*pLock) bIrqIdling ? ThrIdle() : ThrYield();
	*pLock = 1;
}

//! \brief Releases a lock.
static inline void ThrReleaseLock(lock_t* pLock)
{
	*pLock = 0;
}

/** @} */
