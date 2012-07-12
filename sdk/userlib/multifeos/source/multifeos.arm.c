#include <multifeos.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>

enum { THREAD_EXIT = BIT(0), THREAD_IRQWAIT = BIT(1), THREAD_EXECBITS = 3, THREAD_HIGHPRIO = BIT(2) };

typedef struct tag_threadSt
{
	jmp_buf ctx;
	word_t* stack;
	struct tag_threadSt* prev;
	struct tag_threadSt* next;
	execstat_t execStat;
	word_t flags;
	union { int rc; word_t irqMask; };
} threadSt;

threadSt _firstThread;

threadSt* curThread = &_firstThread;
int nThreads = 1;
int threadsWaiting = 0;

void __attribute__((noreturn)) __newThread(void* param, threadEP_t entryPoint, word_t* stackPtr);

static irqWaitFunc_t oldIrqWaitFunc;
static void _irqWaitYield(word_t mask);
static threadSt* _irqWaitCheck();

FEOSINIT void initFirstThread()
{
	curThread->execStat = FeOS_GetCurExecStatus();
	curThread->prev = curThread;
	curThread->next = curThread;
	oldIrqWaitFunc = FeOS_SetIRQWaitFunc(_irqWaitYield);
	FeOS_StayResident();
}

thread_t FeOS_CreateThread(word_t stackSize, threadEP_t entryPoint, void* param)
{
	// 32-bit align the stack size
	stackSize += 3;
	stackSize &= ~3;

	byte_t* mem = (byte_t*) malloc(stackSize + sizeof(threadSt));
	if (!mem) return NULL;

	threadSt* t = (threadSt*) (mem + stackSize);
	t->stack = (word_t*) mem;
	t->execStat = FeOS_GetCurExecStatus();
	FeOS_ExecStatusAddRef(t->execStat);
	t->flags = curThread->flags;

	threadSt* insPoint;
	if (curThread != &_firstThread)
		insPoint = curThread;
	else
	{
		if (t->flags & THREAD_HIGHPRIO)
			insPoint = curThread;
		else
			insPoint = curThread->prev;
	}

	t->prev = insPoint;
	t->next = insPoint->next;
	insPoint->next->prev = t;
	insPoint->next = t;
	threadSt* thisThread = curThread;
	curThread = t;
	nThreads ++;

	if (!setjmp(thisThread->ctx))
		__newThread(param, entryPoint, (word_t*) t);

	return (thread_t) t;
}

void _doYield(threadSt* t)
{
	curThread = t;

	FeOS_SetCurExecStatus(curThread->execStat);
	longjmp(curThread->ctx, 1);
}

void FeOS_Yield()
{
	threadSt* t = curThread;

	if (setjmp(t->ctx))
		return;

	/*if (t == &_firstThread)*/ do
	{
		threadSt* t2 = _irqWaitCheck();
		if (t2)
		{
			// Use the thread _irqWaitCheck() told us to yield to
			_doYield(t2);
			return;
		}
	} while (threadsWaiting == nThreads);

	do t = t->next;
	while (t->flags & THREAD_EXECBITS);

	_doYield(t);
}

thread_t FeOS_GetCurrentThread()
{
	return (thread_t) curThread;
}

void FeOS_ExitThread(int rc)
{
	if (curThread == &_firstThread) return;
	curThread->flags |= THREAD_EXIT;
	curThread->rc = rc;
	FeOS_Yield();
}

bool FeOS_IsThreadActive(thread_t hThread)
{
	return (((threadSt*)hThread)->flags & THREAD_EXIT) == 0;
}

void FeOS_FreeThread(thread_t hThread)
{
	if (hThread == (thread_t) &_firstThread) return;
	if (hThread == (thread_t) curThread) return;

	threadSt* t = (threadSt*) hThread;
	if (t->flags & THREAD_IRQWAIT)
		threadsWaiting --;
	t->prev->next = t->next;
	t->next->prev = t->prev;
	free(t->stack);
	FeOS_ExecStatusRelease(t->execStat);
	nThreads --;
}

int FeOS_ThreadJoin(thread_t hThread)
{
	int rc;
	while (FeOS_IsThreadActive(hThread)) FeOS_Idle();
	rc = FeOS_GetThreadRC(hThread);
	FeOS_FreeThread(hThread);
	return rc;
}

void FeOS_SetThreadPrio(thread_t hThread, int prio)
{
	threadSt* t = (threadSt*) hThread;
	word_t flag = (prio == PRIORITY_HIGH) ? THREAD_HIGHPRIO : 0;
	if ((t->flags & flag) == flag) return;
	t->flags &= ~THREAD_HIGHPRIO;
	t->flags |= flag;

	// Can't change position of the first thread
	if (hThread == (thread_t) &_firstThread) return;

	// Pull the thread out of the list
	t->prev->next = t->next;
	t->next->prev = t->prev;

	// Reinsert it depending on the priority
	threadSt* insPoint = (prio == PRIORITY_HIGH) ? &_firstThread : _firstThread.prev;
	t->prev = insPoint;
	t->next = insPoint->next;
	t->next->prev = t;
	t->prev->next = t;
}

int FeOS_GetThreadRC(thread_t hThread)
{
	return ((threadSt*)hThread)->rc;
}

typedef struct
{
	int argc;
	const char** argv;
} execParams;

static int _execAsyncEP(void* _param)
{
	execParams* params = (void*) _param;
	FeOS_ExecStatusRelease(curThread->execStat);
	curThread->execStat = FeOS_ExecStatusCreate();
	if (!curThread->execStat) return -1;
	FeOS_SetCurExecStatus(curThread->execStat);
	int rc = FeOS_Execute(params->argc, params->argv);
	free(_param);
	return rc;
}

thread_t FeOS_CreateProcess(int argc, const char* argv[])
{
	execParams* params = (execParams*) malloc(sizeof(execParams));
	if (!params) return NULL;

	params->argc = argc;
	params->argv = argv;

	thread_t t = FeOS_CreateThread(16*1024, _execAsyncEP, params);
	if (!t)
	{
		free(params);
		return NULL;
	}

	return t;
}

void _irqWaitYield(word_t mask)
{
	threadsWaiting ++;
	curThread->irqMask = mask;
	curThread->flags |= THREAD_IRQWAIT;
	FeOS_Yield();
}

word_t FeOS_NextIRQYield()
{
	threadsWaiting ++;
	curThread->irqMask = 0;
	curThread->flags |= THREAD_IRQWAIT;
	FeOS_Yield();
	return curThread->irqMask;
}

static void _swapPos(threadSt* a, threadSt* b)
{
	if (a == b) return;
	a->next = b->next;
	a->prev = b->prev;
	a->prev->next = a;
	a->next->prev = a;
	b->prev->next = b;
	b->next->prev = b;
}

threadSt* _irqWaitCheck()
{
	if (!threadsWaiting)
		return NULL;

	// Get the interrupt mask. If all other threads are waiting for IRQs, we can then
	// perform the wait for IRQ.
	word_t mask = (threadsWaiting == nThreads) ? FeOS_NextIRQ() : FeOS_CheckPendingIRQs();

	if (!mask) // implies threadsWaiting != nThreads
		return NULL;

	threadSt *nextT = NULL, *curPos = NULL;

	threadSt* t = curThread;
	int i;
	for (i = 0; i < nThreads && threadsWaiting; t = t->next, i ++)
	{
		if (!(t->flags & THREAD_IRQWAIT)) continue;
		bool isNextIRQ = t->irqMask == 0;
		if ((t->irqMask & mask) || isNextIRQ)
		{
			if (isNextIRQ) t->irqMask = mask;
			t->flags &= ~THREAD_IRQWAIT;
			if (t->flags & THREAD_HIGHPRIO)
			{
				if (nextT == NULL)
				{
					nextT = t;
					curPos = t;
				}else
				{
					_swapPos(curPos->next, t);
					curPos = t;
				}
			}
			threadsWaiting --;
		}
	}

	return nextT;
}
