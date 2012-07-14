#include "feos.h"
#include "fxe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#include <malloc.h>
#include <sys/statvfs.h>

void FeOS_swi_DebugPrint(const char*);

instance_t FeOS_swi_LoadModule_ARM7(const char*, int*);
void FeOS_swi_FreeModule_ARM7(instance_t, int);

void* FeOS_FindSymbol(instance_t hinst, const char* sym)
{
	CHK_HINST(hinst);
	return FindInTbl(&GetRuntimeData(hinst)->exp, sym);
}

int __aeabi_idiv(int, int);
int __aeabi_idivmod(int, int);
unsigned int __aeabi_uidiv(unsigned int, unsigned int);
unsigned int __aeabi_uidivmod(unsigned int, unsigned int);

long long __aeabi_ldivmod(long long, long long);
unsigned long long __aeabi_uldivmod(unsigned long long, unsigned long long);

int FeOS_swi_SuspendIRQ_t();
void FeOS_swi_RestoreIRQ_t(int);
void FeOS_swi_DrainWriteBuffer();
void FeOS_swi_WaitForMemAddr(volatile byte_t*, byte_t);

// Compat. functions

static int FeOS_PushExitFunc(FeOSExitFunc func) { return 1; }
static void FeOS_PopExitFunc() { }

// Execution status functions

typedef void* execstat_t;

execstat_t FeOS_ExecStatusCreate();
void FeOS_ExecStatusAddRef(execstat_t hSt);
void FeOS_ExecStatusRelease(execstat_t hSt);
void FeOS_SetCurExecStatus(execstat_t hSt);
execstat_t FeOS_GetCurExecStatus();

typedef int (* systemfunc_t)(const char* command);

systemfunc_t __system;

int system(const char* command)
{
	if (command == NULL) return !!__system;
	return __system(command);
}

const char* FeOS_GetModuleName(instance_t hInst)
{
	CHK_HINST(hInst);
	return GetRuntimeData(hInst)->name;
}

instance_t FeOS_GetModule(const char* name)
{
	sassert(name, ERRSTR_INVALIDPARAM);
	fxe_runtime_header* rh = FeOS_ModuleListFind(name);
	return rh ? rh->hThis : NULL;
}

// Memory/disk usage functions

typedef struct
{
	word_t total, free, used;
} usagestats_t;

bool FeOS_GetDiskStats(usagestats_t* pStats)
{
	struct statvfs fiData;
	if (pStats && statvfs("/", &fiData) >= 0)
	{
		pStats->total = fiData.f_frsize * fiData.f_blocks;
		pStats->free = fiData.f_bsize * fiData.f_bfree;
		pStats->used = pStats->total - pStats->free;
		return true;
	}
	return false;
}

void FeOS_GetMemStats(usagestats_t* pStats)
{
	if (!pStats) return;

	u8 *heapLimit = getHeapLimit(), *heapStart = getHeapStart(), *heapEnd = getHeapEnd();
	struct mallinfo mi = mallinfo();
	pStats->used = mi.uordblks;
	pStats->free = mi.fordblks + (heapLimit - heapEnd);
	pStats->total = heapLimit - heapStart;
}

int FeOS_GetTickCount()
{
	extern volatile int vblankCounter;
	return vblankCounter;
}

int FeOS_GetVersion()
{
	return FEOS_VERSION_PACK;
}

BEGIN_TABLE(FEOSBASE)
	ADD_FUNC_ALIAS(LoadModule, FeOS_LoadModule),
	ADD_FUNC(FeOS_FindSymbol),
	ADD_FUNC_ALIAS(FreeModule, FeOS_FreeModule),
	ADD_FUNC_ALIAS(ModuleLock, FeOS_LockModule),
	ADD_FUNC_ALIAS(ModuleUnlock, FeOS_UnlockModule),
	ADD_FUNC_ALIAS(ModuleExtraGetSize, FeOS_ModuleGetExtraSize),
	ADD_FUNC_ALIAS(ModuleExtraRead, FeOS_ModuleExtraRead),
	ADD_FUNC_ALIAS(ModuleExtraSeek, FeOS_ModuleExtraSeek),
	ADD_FUNC_ALIAS(ModuleExtraTell, FeOS_ModuleExtraTell),
	ADD_FUNC(FeOS_EnumModules),
	ADD_FUNC(FeOS_GetModule),
	ADD_FUNC(FeOS_GetModuleName),
	ADD_FUNC_ALIAS(FeOS_swi_LoadModule_ARM7, FeOS_LoadARM7),
	ADD_FUNC_ALIAS(FeOS_swi_FreeModule_ARM7, FeOS_FreeARM7),
	ADD_FUNC(FeOS_Execute),
	ADD_FUNC(FeOS_WaitForVBlank),
	ADD_FUNC_ALIAS(FeOS_swi_DebugPrint, FeOS_DebugPrint),
	ADD_FUNC_ALIAS(FeOS_swi_DataCacheFlush, FeOS_DataCacheFlush),
	ADD_FUNC_ALIAS(FeOS_swi_DataCacheFlushAll, FeOS_DataCacheFlushAll),
	ADD_FUNC_ALIAS(FeOS_swi_InstrCacheInvalidate, FeOS_InstrCacheInvalidate),
	ADD_FUNC_ALIAS(FeOS_swi_InstrCacheInvalidateAll, FeOS_InstrCacheInvalidateAll),
	ADD_FUNC_ALIAS(FeOS_swi_DrainWriteBuffer, FeOS_DrainWriteBuffer),
	ADD_FUNC_ALIAS(FeOS_swi_WaitForMemAddr, FeOS_WaitForMemAddr),
	ADD_FUNC_ALIAS(FeOS_swi_SuspendIRQ_t, FeOS_SuspendIRQ_t),
	ADD_FUNC_ALIAS(FeOS_swi_RestoreIRQ_t, FeOS_RestoreIRQ_t),
	ADD_FUNC_ALIAS(FeOS_swi_SetSuspendMode, FeOS_SetSuspendMode),
	ADD_FUNC(FeOS_PushExitFunc),
	ADD_FUNC(FeOS_PopExitFunc),
	ADD_FUNC_ALIAS(FeOS_ModuleExit, exit),
	ADD_FUNC_ALIAS(__errno, FeOS_GetErrnoPtr),
	ADD_FUNC(FeOS_GetModuleExidxTbl),
	ADD_FUNC(FeOS_ModuleFromAddress),
	ADD_FUNC(FeOS_ExecStatusCreate),
	ADD_FUNC(FeOS_ExecStatusAddRef),
	ADD_FUNC(FeOS_ExecStatusRelease),
	ADD_FUNC(FeOS_SetCurExecStatus),
	ADD_FUNC(FeOS_GetCurExecStatus),
	ADD_FUNC(FeOS_SetAutoUpdate),
	ADD_FUNC(FeOS_GetAutoUpdate),
	ADD_FUNC(FeOS_GetDiskStats),
	ADD_FUNC(FeOS_GetMemStats),
	ADD_FUNC(FeOS_GetTickCount),
	ADD_FUNC(FeOS_GetVersion),
	ADD_FUNC(__aeabi_idiv),
	ADD_FUNC(__aeabi_idivmod),
	ADD_FUNC(__aeabi_uidiv),
	ADD_FUNC(__aeabi_uidivmod),
	ADD_FUNC(__aeabi_ldivmod),
	ADD_FUNC(__aeabi_uldivmod),

	// setjmp.h
	ADD_FUNC(setjmp),
	ADD_FUNC(longjmp),

	// stdlib.h
	ADD_FUNC(atoi),
	ADD_FUNC(strtol),
	ADD_FUNC(strtoul),
	ADD_FUNC(rand),
	ADD_FUNC(srand),
	ADD_FUNC(malloc),
	ADD_FUNC(memalign),
	ADD_FUNC(calloc),
	ADD_FUNC(free),
	ADD_FUNC(realloc),
	ADD_FUNC(bsearch),
	ADD_FUNC(qsort),
	ADD_FUNC(abs),
	ADD_FUNC(div), // to be replaced with native div hardware
	ADD_FUNC(system),

	// string.h
	ADD_FUNC(strlen),
	ADD_FUNC(strcpy),
	ADD_FUNC(strcat),
	ADD_FUNC(strcmp),
#undef stricmp
	ADD_FUNC_ALIAS(strcasecmp, stricmp),
#define stricmp strcasecmp
	ADD_FUNC(strncpy),
	ADD_FUNC(strncat),
	ADD_FUNC(strncmp),
#undef strnicmp
	ADD_FUNC_ALIAS(strncasecmp, strnicmp),
#define strnicmp strncasecmp
	ADD_FUNC(memcpy),
	ADD_FUNC(memmove),
	ADD_FUNC(memset),
	ADD_FUNC(memcmp),
	ADD_FUNC(memchr),
	ADD_FUNC(strchr),
	ADD_FUNC(strcspn),
	ADD_FUNC(strpbrk),
	ADD_FUNC(strrchr),
	ADD_FUNC(strspn),
	ADD_FUNC(strstr),
	ADD_FUNC(strtok),
	ADD_FUNC(strerror),

	// ctype.h
	ADD_FUNC(isalnum),
	ADD_FUNC(isalpha),
	ADD_FUNC(iscntrl),
	ADD_FUNC(isdigit),
	ADD_FUNC(isgraph),
	ADD_FUNC(islower),
	ADD_FUNC(isprint),
	ADD_FUNC(ispunct),
	ADD_FUNC(isspace),
	ADD_FUNC(isupper),
	ADD_FUNC(isxdigit),
	ADD_FUNC(tolower),
	ADD_FUNC(toupper),

	// time.h
	ADD_FUNC(mktime),
	ADD_FUNC(time),
	ADD_FUNC(asctime),
	ADD_FUNC(ctime),
	ADD_FUNC(gmtime),
	ADD_FUNC(localtime),
	ADD_FUNC(strftime)
END_TABLE(FEOSBASE)

MAKE_FAKEMODULE(FEOSBASE)

static word_t dummy_entrypoint(word_t a, word_t b, word_t c, word_t d)
{
	return FEOS_RC_OK;
}

typedef struct
{
	word_t refcount;
	instance_t curInstance;
	word_t* exitBuf;
	bool exitDone;
} executeStatus_t;

static executeStatus_t defaultExecStatus;
static executeStatus_t* curExecStatus = &defaultExecStatus;

execstat_t FeOS_ExecStatusCreate()
{
	execstat_t pSt = (execstat_t) malloc(sizeof(executeStatus_t));
	if (!pSt) return NULL;

	memset(pSt, 0, sizeof(executeStatus_t));
	((executeStatus_t*)pSt)->refcount = 1;
	return pSt;
}

void FeOS_ExecStatusAddRef(execstat_t hSt)
{
	if (hSt == (execstat_t) &defaultExecStatus) return;
	((executeStatus_t*)hSt)->refcount ++;
}

void FeOS_ExecStatusRelease(execstat_t hSt)
{
	if (hSt == (execstat_t) &defaultExecStatus) return;
	word_t r = --((executeStatus_t*)hSt)->refcount;
	if (r == 0)
		free(hSt);
}

void FeOS_SetCurExecStatus(execstat_t hSt)
{
	curExecStatus = (executeStatus_t*) hSt;
}

execstat_t FeOS_GetCurExecStatus()
{
	return (execstat_t) curExecStatus;
}

/*
instance_t FeOS_CurInstance = NULL;
static word_t* FeOS_ExitBuf;
static bool FeOS_ExitDone = false;
*/

#define FeOS_CurInstance curExecStatus->curInstance
#define FeOS_ExitBuf     curExecStatus->exitBuf
#define FeOS_ExitDone    curExecStatus->exitDone

int FeOS_Execute(int argc, const char* argv[])
{
	if (argc == 0) return E_INVALIDARG;
	instance_t hInst = LoadModule(argv[0]);
	if (!hInst) return E_FILENOTFOUND;

	if (!__system)
		__system = (systemfunc_t) FeOS_FindSymbol(hInst, "__system");

	fxe_runtime_header* rh = GetRuntimeData(hInst);

	int rc = E_INVALIDARG;
	if (rh->entrypoint)
	{
		instance_t instbak = FeOS_CurInstance;
		FeOS_CurInstance = hInst;

		jmp_buf exitbuf;
		word_t* exitbufbak = FeOS_ExitBuf;
		FeOS_ExitBuf = exitbuf;
		rc = setjmp(exitbuf);
		if (!FeOS_ExitDone)
			rc = rh->entrypoint(FEOS_EP_MAIN, (word_t)argc, (word_t)argv, 0);
		else
			FeOS_ExitDone = false;

		FeOS_ExitBuf = exitbufbak;
		FeOS_CurInstance = instbak;
	}

	// If the app was forcefully killed we don't want FreeModule() to run the destructors
	if ((int)rc == E_APPKILLED) rh->entrypoint = dummy_entrypoint;

	FreeModule(hInst);
	return rc;
}

void FeOS_ModuleExit(int rc)
{
	FeOS_ExitDone = true;
	longjmp0(FeOS_ExitBuf, rc);
}

void* FeOS_GetModuleExidxTbl(instance_t hInst, int* count)
{
	if (hInst == (instance_t)(~0))
	{
		// get exidx for kernel itself
		typedef struct { word_t offset, size; } exidx_entry_t;
		extern exidx_entry_t __exidx_start;
		extern exidx_entry_t __exidx_end;
		*count = &__exidx_end - &__exidx_start;
		return &__exidx_start;
	}

	CHK_HINST(hInst);
	fxe_runtime_header* rh = GetRuntimeData(hInst);

	if (!rh->exidx.nentries) return NULL;
	if (count) *count = rh->exidx.nentries;

	return rh->exidx.table;
}

void FeOS_DataCacheFlush(const void* mem, size_t size)
{
	DC_FlushRange(mem, size);
}

void FeOS_DataCacheFlushAll()
{
	DC_FlushAll();
}

void FeOS_InstrCacheInvalidate(const void* mem, size_t size)
{
	IC_InvalidateRange(mem, size);
}

void FeOS_InstrCacheInvalidateAll()
{
	IC_InvalidateAll();
}
