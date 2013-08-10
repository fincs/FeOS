#include "thread.h"
#include "idmgr.h"

threadSt _firstThread;
idmgr_t _tlsIds;

threadSt* curThread = &_firstThread;
int nThreads = 1;
int threadsWaiting = 0;
int threadsFinished = 0;

#define inactiveThreadCount() (threadsWaiting + threadsFinished)

void __attribute__((noreturn)) __enterThread(void* param, threadEP_t entryPoint, word_t* stackPtr);

static threadSt* _irqWaitCheck();

void ThrInit()
{
	curThread->execStat = KeGetCurExecStatus();
	curThread->prev = curThread;
	curThread->next = curThread;
	IdMgrInit(&_tlsIds);
}

thread_t ThrCreate(word_t stackSize, threadEP_t entryPoint, void* param)
{
	// 32-bit align the stack size
	stackSize += 3;
	stackSize &= ~3;

	byte_t* mem = (byte_t*) malloc(stackSize + sizeof(threadSt));
	if (!mem) return NULL;

	threadSt* t = (threadSt*) (mem + stackSize);
	t->stack = (word_t*) mem;
	t->execStat = KeGetCurExecStatus();
	KeExecStatusAddRef(t->execStat);
	t->flags = curThread->flags & THREAD_HIGHPRIO;

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
		__enterThread(param, entryPoint, (word_t*) t);

	return t;
}

void _doYield(threadSt* t)
{
	curThread = t;

	KeSetCurExecStatus(curThread->execStat);
	longjmp(curThread->ctx, 1);
}

void ThrYield()
{
	threadSt* t = curThread;

	if (setjmp(t->ctx))
		return;

	do
	{
		threadSt* t2 = _irqWaitCheck();
		if (t2)
		{
			// Use the thread _irqWaitCheck() told us to yield to
			_doYield(t2);
			return;
		}
	} while (inactiveThreadCount() == nThreads);

	do
	{
		t = t->next;

		while ((t->flags & THREAD_DETACHED) && (t->flags & THREAD_EXIT))
		{
			threadSt* next = t->next;
			ThrFree(t);
			t = next;
		}
	} while (t->flags & THREAD_EXECBITS);

	_doYield(t);
}

thread_t ThrGetSelf()
{
	return curThread;
}

void ThrExit(int rc)
{
	if (curThread == &_firstThread) return;
	curThread->flags |= THREAD_EXIT;
	curThread->rc = rc;
	threadsFinished ++;
	ThrYield();
}

bool ThrIsActive(thread_t hThread)
{
	return (hThread->flags & THREAD_EXIT) == 0;
}

void ThrFree(thread_t t)
{
	if (t == &_firstThread) return;
	if (t == curThread) return;

	if (t->flags & THREAD_IRQWAIT)
		threadsWaiting --;
	if (t->flags & THREAD_EXIT)
		threadsFinished --;
	t->prev->next = t->next;
	t->next->prev = t->prev;
	free(t->stack);
	KeExecStatusRelease(t->execStat);
	nThreads --;
}

int ThrJoin(thread_t hThread)
{
	int rc;
	while (ThrIsActive(hThread)) DSWaitForIRQ(~0);
	rc = ThrGetExitCode(hThread);
	ThrFree(hThread);
	return rc;
}

void ThrSetPriority(thread_t t, int prio)
{
	word_t flag = (prio == PRIORITY_HIGH) ? THREAD_HIGHPRIO : 0;
	if ((t->flags & flag) == flag) return;
	t->flags &= ~THREAD_HIGHPRIO;
	t->flags |= flag;

	// Can't change position of the first thread
	if (t == &_firstThread) return;

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

int ThrGetExitCode(thread_t hThread)
{
	return hThread->rc;
}

typedef struct
{
	int argc;
	const char** argv;
} execParams;

static int _execAsyncEP(void* _param)
{
	execParams* params = (void*) _param;
	execstat_t es = KeExecStatusCreate();
	if (!es) return -4;
	KeExecStatusRelease(curThread->execStat);
	curThread->execStat = es;
	KeSetCurExecStatus(curThread->execStat);
	int rc = LdrExecuteArgv(params->argc, params->argv);
	free(_param);
	return rc;
}

thread_t PsCreateFromArgv(int argc, const char* argv[])
{
	execParams* params = (execParams*) malloc(sizeof(execParams));
	if (!params) return NULL;

	params->argc = argc;
	params->argv = argv;

	thread_t t = ThrCreate(16*1024, _execAsyncEP, params);
	if (!t)
	{
		free(params);
		return NULL;
	}

	return t;
}

static int _runAsyncEP(void* _param)
{
	execstat_t es = KeExecStatusCreate();
	if (!es) return -4;
	KeExecStatusRelease(curThread->execStat);
	curThread->execStat = es;
	if (!curThread->execStat) return -1;
	KeSetCurExecStatus(curThread->execStat);
	return system((const char*)_param);
}

thread_t PsCreateFromCmdLine(const char* command)
{
	return ThrCreate(16*1024, _runAsyncEP, (void*)command);
}

void ThrDetach(thread_t t)
{
	if (t == &_firstThread || t == curThread)
		return;
	t->flags |= THREAD_DETACHED;
}

int ThrRunInContext(thread_t t, threadEP_t func, void* param)
{
	execstat_t oldSt = curThread->execStat;
	curThread->execStat = t->execStat;
	KeExecStatusAddRef(curThread->execStat);
	KeSetCurExecStatus(curThread->execStat);
	int rc = func(param);
	curThread->execStat = oldSt;
	KeSetCurExecStatus(curThread->execStat);
	KeExecStatusRelease(curThread->execStat);
	return rc;
}

void DSWaitForIRQ(word_t mask)
{
	threadsWaiting ++;
	curThread->irqMask = mask;
	curThread->flags |= THREAD_IRQWAIT;
	ThrYield();
}

word_t DSWaitForNextIRQ()
{
	threadsWaiting ++;
	curThread->irqMask = 0;
	curThread->flags |= THREAD_IRQWAIT;
	ThrYield();
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

	// Get the interrupt mask. If all other threads are inactive (such as waiting for IRQs),
	// we can then perform the wait for IRQ.
	word_t mask = (inactiveThreadCount() == nThreads) ? DSWaitForNextIRQRaw() : DSProcessIRQs();

	if (!mask) // implies inactiveThreadCount() != nThreads
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

int ThrTlsAlloc()
{
	return IdMgrAlloc(&_tlsIds);
}

void ThrTlsFree(int id)
{
	IdMgrFree(&_tlsIds, id);
}

void* ThrTlsGetValue(int id)
{
	return curThread->tls[id];
}

void ThrTlsSetValue(int id, void* value)
{
	curThread->tls[id] = value;
}
