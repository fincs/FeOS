#include "feos.h"
#include "loader.h"
#include "feosfifo.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#include <malloc.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

static inline int _fileno_helper(FILE* f) { return fileno(f); }

void* LdrFindSymbol(module_t hMod, const char* sym)
{
	CHK_HMOD(hMod);
	return LdrFindInTbl(&GetRuntimeData(hMod)->exp, sym);
}

int __aeabi_idiv(int, int);
int __aeabi_idivmod(int, int);
unsigned int __aeabi_uidiv(unsigned int, unsigned int);
unsigned int __aeabi_uidivmod(unsigned int, unsigned int);

long long __aeabi_ldivmod(long long, long long);
unsigned long long __aeabi_uldivmod(unsigned long long, unsigned long long);

void DC_DrainWriteBuffer();
void KeWaitForMemAddr(volatile byte_t*, byte_t);

void KeSetExcptHandler(void*);

// Execution status functions

typedef int (* systemfunc_t)(const char* command);

systemfunc_t __system;

int system(const char* command)
{
	if (command == NULL) return !!__system;
	return __system(command);
}

const char* LdrGetModuleName(module_t hMod)
{
	CHK_HMOD(hMod);
	return GetRuntimeData(hMod)->name;
}

module_t LdrGetModule(const char* name)
{
	sassert(name, ERRSTR_INVALIDPARAM);
	fxe_runtime_header* rh = LdrModuleListFind(name);
	return rh ? rh->hThis : NULL;
}

// Memory/disk usage functions

typedef struct
{
	word_t total, free, used;
} usagestats_t;

typedef struct
{
	u64 total, free, used;
} usagestats64_t;

bool KeGetDiskStats(usagestats64_t* pStats)
{
	struct statvfs fiData;
	if (pStats && statvfs("/", &fiData) >= 0)
	{
		pStats->total = (u64)fiData.f_frsize * fiData.f_blocks;
		pStats->free = (u64)fiData.f_bsize * fiData.f_bfree;
		pStats->used = pStats->total - pStats->free;
		return true;
	}
	return false;
}

void KeGetMemStats(usagestats_t* pStats)
{
	if (!pStats) return;

	u8 *heapLimit = getHeapLimit(), *heapStart = getHeapStart(), *heapEnd = getHeapEnd();
	struct mallinfo mi = mallinfo();
	pStats->used = mi.uordblks;
	pStats->free = mi.fordblks + (heapLimit - heapEnd);
	pStats->total = heapLimit - heapStart;
}

int KeGetTickCount()
{
	extern volatile int vblankCounter;
	return vblankCounter;
}

int KeGetOSVersion()
{
	return FEOS_VERSION_PACK;
}

static systeminfo_t sysInfo =
{
	FEOS_VERSION_PACK,
	FEOS_VERSION_TEXT,
#ifndef DEBUG
	0,
#else
	SYSTEM_FLAGS_DEBUG,
#endif
	SYSTEM_TYPE_DS,
	NULL,
	SYSTEM_MODE_DS,
	NULL,
};

void KeInitSystemInfo()
{
	extern bool __dsimode;

#define SYSTYPE sysInfo.systemType
#define SYSFLAGS sysInfo.systemFlags

	// DSi mode detection
	if (__dsimode)
	{
		sysInfo.systemMode = SYSTEM_MODE_DSi;
		sysInfo.systemType = SYSTEM_TYPE_DSi;
	}

	byte_t fw1D, fw2F, pm4;
	// Retrieve some bits from ARM7
	{
		FeOSFifoMsg msg;
		msg.type = FEOS_ARM7_GET_SYSTEM_INFO;

		fifoSendDatamsg(FIFO_FEOS, sizeof(msg), (void*) &msg);
		while (!fifoCheckValue32(FIFO_FEOS));

		word_t ret = fifoGetValue32(FIFO_FEOS);
		fw1D = ret;
		fw2F = ret >> 8;
		pm4 = ret >> 16;
	}

	// Detect system type
	// Based on http://adshomebrewersdiary.blogspot.com.es/2012/01/telling-ds-models-apart.html

	if (SYSTYPE == SYSTEM_TYPE_DS) do
	{
		if (swiIsDebugger())
		{
			SYSTYPE = SYSTEM_TYPE_DSDebug;
			break;
		}

		if (pm4 & 0x80)
		{
			SYSTYPE = SYSTEM_TYPE_DSLite;
			break;
		}

		if (fw1D != 0xFF && fw1D & 3)
			SYSFLAGS |= SYSTEM_FLAGS_CHINESE;
	} while(0);

	if (SYSTYPE == SYSTEM_TYPE_DSLite) do
	{
		if (fw1D & 4)
		{
			SYSTYPE = SYSTEM_TYPE_DSi;
			break;
		}

		if (fw1D & 3)
			SYSFLAGS |= SYSTEM_FLAGS_CHINESE;
	} while(0);

	if (SYSTYPE == SYSTEM_TYPE_DSi) do
	{
		if (fw2F >= 0x18)
		{
			SYSTYPE = SYSTEM_TYPE_DSi_XL;
			break;
		}

		// TODO: chinese DSi detection
	} while(0);

	if (SYSTYPE == SYSTEM_TYPE_DSi_XL) do
	{
		if (fw2F >= 0x1C)
		{
			SYSTYPE = SYSTEM_TYPE_3DS;
			break;
		}

		// TODO: chinese DSi XL detection
	} while(0);

	// TODO: 3DS XL detection
	// TODO: chinese 3DS XL detection (does that even exist??)

	// Emulator detection
	bool __isEmulator();
	if (__isEmulator())
		SYSFLAGS |= SYSTEM_FLAGS_EMULATOR;

#undef SYSTYPE
#undef SYSFLAGS

	switch (sysInfo.systemType)
	{
#define _TEXT(_x,_y) case _x: sysInfo.systemTypeText = _y; break
		_TEXT(SYSTEM_TYPE_DS, "Nintendo DS (Original)");
		_TEXT(SYSTEM_TYPE_DSDebug, "Nintendo DS (Debug)");
		_TEXT(SYSTEM_TYPE_DSLite, "Nintendo DS Lite");
		_TEXT(SYSTEM_TYPE_DSi, "Nintendo DSi");
		_TEXT(SYSTEM_TYPE_DSi_XL, "Nintendo DSi XL");
		_TEXT(SYSTEM_TYPE_3DS, "Nintendo 3DS");
		_TEXT(SYSTEM_TYPE_3DS_XL, "Nintendo 3DS XL");
#undef _TEXT
	}

	switch (sysInfo.systemMode)
	{
#define _TEXT(_x,_y) case _x: sysInfo.systemModeText = _y; break
		_TEXT(SYSTEM_MODE_DS, "Standard DS Mode (67.03 MHz)");
		_TEXT(SYSTEM_MODE_DSi, "Enhanced DSi Mode (134.06 MHz)");
#undef _TEXT
	}
}

const systeminfo_t* KeGetSysInfo()
{
	return &sysInfo;
}

static inline void divmod3232(int a, int b)
{
	REG_DIVCNT = DIV_32_32;
	while(REG_DIVCNT & DIV_BUSY);

	REG_DIV_NUMER_L = a;
	REG_DIV_DENOM_L = b;
	while(REG_DIVCNT & DIV_BUSY);
}

static inline void divmod6432(long long a, int b)
{
	REG_DIVCNT = DIV_64_32;
	while(REG_DIVCNT & DIV_BUSY);

	REG_DIV_NUMER   = a;
	REG_DIV_DENOM_L = b;
	while(REG_DIVCNT & DIV_BUSY);
}

static inline void divmod6464(long long a, long long b)
{
	REG_DIVCNT = DIV_64_64;
	while(REG_DIVCNT & DIV_BUSY);

	REG_DIV_NUMER = a;
	REG_DIV_DENOM = b;
	while(REG_DIVCNT & DIV_BUSY);
}

int KeDiv3232(int a, int b)
{
	divmod3232(a, b);
	return REG_DIV_RESULT_L;
}

int KeMod3232(int a, int b)
{
	divmod3232(a, b);
	return REG_DIVREM_RESULT_L;
}

int KeDiv6432(long long a, int b)
{
	divmod6432(a, b);
	return REG_DIV_RESULT_L;
}

int KeMod6432(long long a, int b)
{
	divmod6432(a, b);
	return REG_DIVREM_RESULT_L;
}

long long KeDiv6464(long long a, long long b)
{
	divmod6464(a, b);
	return REG_DIV_RESULT;
}

long long KeMod6464(long long a, long long b)
{
	divmod6464(a, b);
	return REG_DIVREM_RESULT;
}

unsigned int KeSqrt32(unsigned int a)
{
	REG_SQRTCNT = SQRT_32;
	while(REG_SQRTCNT & SQRT_BUSY);

	REG_SQRT_PARAM_L = a;
	while(REG_SQRTCNT & SQRT_BUSY);

	return REG_SQRT_RESULT;
}

unsigned int KeSqrt64(long long a)
{
	REG_SQRTCNT = SQRT_64;
	while(REG_SQRTCNT & SQRT_BUSY);

	REG_SQRT_PARAM = a;
	while(REG_SQRTCNT & SQRT_BUSY);

	return REG_SQRT_RESULT;
}

#define IoStdinHook curExecStatus->stdin_hook
#define IoStdoutHook curExecStatus->stdout_hook
#define IoStderrHook curExecStatus->stderr_hook

FILE* IoGetStdin()
{
	return !IoStdinHook ? stdin : (FILE*)IoStdinHook;
}

FILE* IoGetStdout()
{
	return !IoStdoutHook ? stdout : (FILE*)IoStdoutHook;
}

FILE* IoGetStderr()
{
	return !IoStderrHook ? stderr : (FILE*)IoStderrHook;
}

FILE* IoSetStdin(FILE* newstdin)
{
	FILE* oldstdin = (FILE*)IoStdinHook;
	IoStdinHook = newstdin;
	return oldstdin;
}

FILE* IoSetStdout(FILE* newstdout)
{
	FILE* oldstdout = (FILE*)IoStdoutHook;
	IoStdoutHook = newstdout;
	return oldstdout;
}

FILE* IoSetStderr(FILE* newstderr)
{
	FILE* oldstderr = (FILE*)IoStderrHook;
	IoStderrHook = newstderr;
	return oldstderr;
}

FILE* IoOpenStream(const void*);

#undef stricmp
#undef strnicmp

BEGIN_TABLE(FEOSKRNL)
	ADD_FUNC_(DC_DrainWriteBuffer),
	ADD_ALIAS(DC_FlushAll, KeDataCacheFlushAll),
	ADD_ALIAS(DC_FlushRange, KeDataCacheFlush),
	ADD_FUNC_(DSGetAutoUpdate),
	ADD_FUNC_(DSSetAutoUpdate),
	ADD_ALIAS(IC_InvalidateAll, KeInstrCacheInvalidateAll),
	ADD_ALIAS(IC_InvalidateRange, KeInstrCacheInvalidate),
	ADD_FUNC_(IoGetStderr),
	ADD_FUNC_(IoGetStdin),
	ADD_FUNC_(IoGetStdout),
	ADD_FUNC_(IoOpenStream),
	ADD_FUNC_(IoSetStderr),
	ADD_FUNC_(IoSetStdin),
	ADD_FUNC_(IoSetStdout),
	ADD_FUNC_(KeDiv3232),
	ADD_FUNC_(KeDiv6432),
	ADD_FUNC_(KeDiv6464),
	ADD_FUNC_(KeGetDiskStats),
	ADD_ALIAS(KeGetErrnoPtr, __errno),
	ADD_FUNC_(KeGetMemStats),
	ADD_FUNC_(KeGetOSVersion),
	ADD_FUNC_(KeGetSysInfo),
	ADD_FUNC_(KeGetTickCount),
	ADD_FUNC_(KeMod3232),
	ADD_FUNC_(KeMod6432),
	ADD_FUNC_(KeMod6464),
	ADD_FUNC_(KeSetExcptHandler),
	ADD_FUNC_(KeSqrt32),
	ADD_FUNC_(KeSqrt64),
	ADD_FUNC_(KeWaitForMemAddr),
	ADD_FUNC_(LdrEnumModules),
	ADD_FUNC_(LdrExecuteArgv),
	ADD_FUNC_(LdrFindSymbol),
	ADD_FUNC_(LdrFreeModule),
	ADD_FUNC_(LdrGetExidxTbl),
	ADD_FUNC_(LdrGetModule),
	ADD_FUNC_(LdrGetModuleName),
	ADD_FUNC_(LdrLoadModule),
	ADD_FUNC_(LdrLockModule),
	ADD_FUNC_(LdrResGetSize),
	ADD_FUNC_(LdrResRead),
	ADD_FUNC_(LdrResSeek),
	ADD_FUNC_(LdrResTell),
	ADD_FUNC_(LdrResolveAddr),
	ADD_FUNC_(LdrUnlockModule),
	ADD_FUNC_(PsCreateFromArgv),
	ADD_FUNC_(PsCreateFromCmdLine),
	ADD_FUNC_(ThrCreate),
	ADD_FUNC_(ThrDetach),
	ADD_FUNC_(ThrExit),
	ADD_FUNC_(ThrFree),
	ADD_FUNC_(ThrGetExitCode),
	ADD_FUNC_(ThrGetSelf),
	ADD_FUNC_(ThrIsActive),
	ADD_FUNC_(ThrJoin),
	ADD_FUNC_(ThrRunInContext),
	ADD_FUNC_(ThrSetPriority),
	ADD_FUNC_(ThrYield),
	ADD_FUNC_(__aeabi_idiv),
	ADD_FUNC_(__aeabi_idivmod),
	ADD_FUNC_(__aeabi_ldivmod),
	ADD_FUNC_(__aeabi_uidiv),
	ADD_FUNC_(__aeabi_uidivmod),
	ADD_FUNC_(__aeabi_uldivmod),
	ADD_FUNC_(abs),
	ADD_FUNC_(acosLerp),
	ADD_FUNC_(asctime),
	ADD_FUNC_(asinLerp),
	ADD_FUNC_(atoi),
	ADD_FUNC_(bsearch),
	ADD_FUNC_(calloc),
	ADD_FUNC_(chdir),
	ADD_FUNC_(clearerr),
	ADD_FUNC_(close),
	ADD_FUNC_(closedir),
	ADD_FUNC_(cosLerp),
	ADD_FUNC_(ctime),
	ADD_FUNC_(div),
	ADD_ALIAS(exit, LdrModuleExit),
	ADD_FUNC_(fclose),
	ADD_FUNC_(fdopen),
	ADD_FUNC_(feof),
	ADD_FUNC_(ferror),
	ADD_FUNC_(fflush),
	ADD_FUNC_(fgetc),
	ADD_FUNC_(fgets),
	ADD_ALIAS(fileno, _fileno_helper),
	ADD_FUNC_(fopen),
	ADD_FUNC_(fputc),
	ADD_FUNC_(fputs),
	ADD_FUNC_(fread),
	ADD_FUNC_(free),
	ADD_FUNC_(freopen),
	ADD_FUNC_(fseek),
	ADD_FUNC_(fstat),
	ADD_FUNC_(ftell),
	ADD_FUNC_(ftruncate),
	ADD_FUNC_(fwrite),
	ADD_FUNC_(getcwd),
	ADD_ALIAS(getdelim, __getdelim),
	ADD_FUNC_(gmtime),
	ADD_FUNC_(isalnum),
	ADD_FUNC_(isalpha),
	ADD_FUNC_(iscntrl),
	ADD_FUNC_(isdigit),
	ADD_FUNC_(isgraph),
	ADD_FUNC_(islower),
	ADD_FUNC_(isprint),
	ADD_FUNC_(ispunct),
	ADD_FUNC_(isspace),
	ADD_FUNC_(isupper),
	ADD_FUNC_(isxdigit),
	ADD_FUNC_(localtime),
	ADD_FUNC_(longjmp),
	ADD_FUNC_(lseek),
	ADD_FUNC_(malloc),
	ADD_FUNC_(memalign),
	ADD_FUNC_(memchr),
	ADD_FUNC_(memcmp),
	ADD_FUNC_(memcpy),
	ADD_FUNC_(memmove),
	ADD_FUNC_(memset),
	ADD_FUNC_(mkdir),
	ADD_FUNC_(mktime),
	ADD_FUNC_(open),
	ADD_FUNC_(opendir),
	ADD_FUNC_(qsort),
	ADD_FUNC_(rand),
	ADD_FUNC_(read),
	ADD_FUNC_(readdir),
	ADD_FUNC_(realloc),
	ADD_FUNC_(remove),
	ADD_FUNC_(rename),
	ADD_FUNC_(rewinddir),
	ADD_FUNC_(seekdir),
	ADD_FUNC_(setjmp),
	ADD_FUNC_(setvbuf),
	ADD_FUNC_(sinLerp),
	ADD_FUNC_(srand),
	ADD_FUNC_(stat),
	ADD_FUNC_(strcat),
	ADD_FUNC_(strchr),
	ADD_FUNC_(strcmp),
	ADD_FUNC_(strcpy),
	ADD_FUNC_(strcspn),
	ADD_FUNC_(strerror),
	ADD_FUNC_(strftime),
	ADD_ALIAS(stricmp, strcasecmp),
	ADD_FUNC_(strlen),
	ADD_FUNC_(strncat),
	ADD_FUNC_(strncmp),
	ADD_FUNC_(strncpy),
	ADD_ALIAS(strnicmp, strncasecmp),
	ADD_FUNC_(strpbrk),
	ADD_FUNC_(strrchr),
	ADD_FUNC_(strspn),
	ADD_FUNC_(strstr),
	ADD_FUNC_(strtok),
	ADD_FUNC_(strtol),
	ADD_FUNC_(strtoul),
	ADD_FUNC_(system),
	ADD_FUNC_(tanLerp),
	ADD_FUNC_(telldir),
	ADD_FUNC_(time),
	ADD_FUNC_(tolower),
	ADD_FUNC_(toupper),
	ADD_FUNC_(ungetc),
	ADD_ALIAS(vfprintf, vfiprintf),
	ADD_ALIAS(vfscanf, vfiscanf),
	ADD_ALIAS(vsnprintf, vsniprintf),
	ADD_ALIAS(vsprintf, vsiprintf),
	ADD_ALIAS(vsscanf, vsiscanf),
	ADD_FUNC_(write),
END_TABLE(FEOSKRNL)

MAKE_FAKEMODULE(FEOSKRNL)

#define stricmp strcasecmp
#define strnicmp strncasecmp

static word_t dummy_entrypoint(word_t a, word_t b, word_t c, word_t d)
{
	return FEOS_RC_OK;
}

static executeStatus_t defaultExecStatus;
executeStatus_t* curExecStatus = &defaultExecStatus;

#ifdef LIBFAT_FEOS_MULTICWD
void KeInitDefaultExecStatus()
{
	defaultExecStatus.cwdBuffer = _getCwdBuf();
}

#define EXECSTAT_ADDENDUM 256
#else
#define EXECSTAT_ADDENDUM 0
#endif

#define GET_ADDENDUM(x) ((char*)(x) + sizeof(executeStatus_t));

execstat_t KeExecStatusCreate()
{
	executeStatus_t* pSt = (executeStatus_t*) malloc(sizeof(executeStatus_t) + EXECSTAT_ADDENDUM);
	if (!pSt) return NULL;

	memset(pSt, 0, sizeof(executeStatus_t));
	pSt->refcount = 1;

	// Inherit standard stream hooks
	pSt->stdin_hook = curExecStatus->stdin_hook;
	pSt->stdout_hook = curExecStatus->stdout_hook;
	pSt->stderr_hook = curExecStatus->stderr_hook;

#ifdef LIBFAT_FEOS_MULTICWD
	// Inherit current working directory
	pSt->cwdCluster = g_fatCwdCluster;
	pSt->cwdBuffer = GET_ADDENDUM(pSt);
	memcpy(pSt->cwdBuffer, curExecStatus->cwdBuffer, EXECSTAT_ADDENDUM);
#endif

	return pSt;
}

void KeExecStatusAddRef(execstat_t hSt)
{
	if (hSt == &defaultExecStatus) return;
	hSt->refcount ++;
}

void KeExecStatusRelease(execstat_t hSt)
{
	if (hSt == &defaultExecStatus) return;
	word_t r = --hSt->refcount;
	if (r == 0)
		free(hSt);
}

void KeSetCurExecStatus(execstat_t hSt)
{
#ifdef LIBFAT_FEOS_MULTICWD
	curExecStatus->cwdCluster = g_fatCwdCluster;
#endif
	curExecStatus = hSt;
#ifdef LIBFAT_FEOS_MULTICWD
	g_fatCwdCluster = curExecStatus->cwdCluster;
	_setCwdBuf(curExecStatus->cwdBuffer);
#endif
}

execstat_t KeGetCurExecStatus()
{
	return curExecStatus;
}

#define LdrCurModule curExecStatus->curModule
#define LdrExitBuf   curExecStatus->exitBuf
#define LdrExitDone  curExecStatus->exitDone

int LdrExecuteArgv(int argc, const char* argv[])
{
	if (argc == 0) return E_INVALIDARG;
	module_t hMod = LdrLoadModule(argv[0]);
	if (!hMod) return E_FILENOTFOUND;

	if (!__system)
		__system = (systemfunc_t) LdrFindSymbol(hMod, "__system");

	fxe_runtime_header* rh = GetRuntimeData(hMod);

	int rc = E_INVALIDARG;
	if (rh->entrypoint)
	{
		module_t hPrevMod = LdrCurModule;
		LdrCurModule = hMod;

		jmp_buf exitbuf;
		word_t* pPrevBuf = LdrExitBuf;
		LdrExitBuf = exitbuf;
		rc = setjmp(exitbuf);
		if (!LdrExitDone)
			rc = rh->entrypoint(FEOS_EP_MAIN, (word_t)argc, (word_t)argv, 0);
		else
			LdrExitDone = false;

		LdrExitBuf = pPrevBuf;
		LdrCurModule = hPrevMod;
	}

	// If the app was forcefully killed we don't want FreeModule() to run the destructors
	if ((int)rc == E_APPKILLED) rh->entrypoint = dummy_entrypoint;

	LdrFreeModule(hMod);
	return rc;
}

void LdrModuleExit(int rc)
{
	LdrExitDone = true;
	longjmp0(LdrExitBuf, rc);
}

void* LdrGetExidxTbl(module_t hMod, int* count)
{
	if (hMod == (module_t)(~0))
	{
		// get exidx for kernel itself
		typedef struct { word_t offset, size; } exidx_entry_t;
		extern exidx_entry_t __exidx_start;
		extern exidx_entry_t __exidx_end;
		*count = &__exidx_end - &__exidx_start;
		return &__exidx_start;
	}

	CHK_HMOD(hMod);
	fxe_runtime_header* rh = GetRuntimeData(hMod);

	if (!rh->exidx.nentries) return NULL;
	if (count) *count = rh->exidx.nentries;

	return rh->exidx.table;
}
