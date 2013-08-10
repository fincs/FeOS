#pragma once
#include "feos.h"
#include "loader.h"

#define PRIORITY_HIGH   0x7FFFFFFF
#define PRIORITY_NORMAL 0

enum { THREAD_EXIT = BIT(0), THREAD_IRQWAIT = BIT(1), THREAD_EXECBITS = 3, THREAD_HIGHPRIO = BIT(2), THREAD_DETACHED = BIT(3) };

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

typedef threadSt* thread_t;
typedef int (*threadEP_t)(void* param);

void ThrInit();

thread_t ThrCreate(word_t stackSize, threadEP_t entryPoint, void* param);
void ThrYield();
thread_t ThrGetSelf();
void ThrExit(int rc);
bool ThrIsActive(thread_t hThread);
void ThrFree(thread_t hThread);
void ThrSetPriority(thread_t hThread, int prio);
int ThrGetExitCode(thread_t hThread);
int ThrJoin();
void ThrDetach(thread_t hThread);
int ThrRunInContext(thread_t hThread, threadEP_t func, void* param);

thread_t PsCreateFromArgv(int argc, const char* argv[]);
thread_t PsCreateFromCmdLine(const char* command);
