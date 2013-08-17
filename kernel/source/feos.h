#pragma once
#include <nds.h>
#include "../../sdk/include/feosver.h"

#ifdef ARM9
#include <fat.h>
#define ITCM_DATA __attribute__((section(".itcm")))
#endif

#define KeAllocStack(a) __builtin_alloca(a)

#ifdef ARM9
#define BEGIN_TABLE(_NAME_) static const fxe2_export_t _exp_##_NAME_##_tbl[] = {
#define ADD_FUNC_(FUNC) {{(word_t)#FUNC}, {(word_t)FUNC}}
#define ADD_ALIAS(NAME, FUNC) {{(word_t)#NAME}, {(word_t)FUNC}}
#define END_TABLE(_NAME_) };
#define MAKE_EXPORTSTRUCT(_NAME_) { sizeof(_exp_##_NAME_##_tbl) / sizeof(fxe2_export_t), (fxe2_export_t*) _exp_##_NAME_##_tbl }

#define MAKE_FAKEMODULE(_NAME_) \
	extern void* _inst_##_NAME_; \
	\
	fxe_runtime_header _header_##_NAME_ = \
	{ \
		&_inst_##_NAME_, /* hThis */ \
		#_NAME_, /* name */ \
		1, /* refcount */ \
		-1, /* file */ \
		0, /* size */ \
		0, /* extrapos */ \
		0, /* extrasize */ \
		NULL, /* entrypoint */ \
		MAKE_EXPORTSTRUCT(_NAME_), /* exp */ \
		{ 0, NULL }, /* imp */ \
		{ NULL, 0 }, /* exidx */ \
		NULL, /* next */ \
		NULL /* prev */ \
	}; \
	\
	void* _inst_##_NAME_ = &_header_##_NAME_;
#endif

typedef unsigned int word_t;
typedef unsigned short hword_t;
typedef unsigned char byte_t;
typedef word_t bool_t;
typedef void* module_t;
typedef int (*threadEP_t)(void* param);

#ifdef ARM9
#include "feos_setjmp.h"

#define IDMGR_MAXIDS 64

void KeEnterUserMode();
void KeInitUserMode();

word_t __ARMSWP(word_t value, volatile word_t* addr);

extern bool_t __inFAT;

static inline bool AddressCheckMainRAM(const void* addr)
{
	return (word_t)addr >= 0x02000000 && (word_t)addr <= 0x03000000;
}

#define E_FILENOTFOUND (-10)
#define E_INVALIDARG (-11)
#define E_APPKILLED (-12)

void DSRunFifoQueue();
void DSFifoSetDatamsgHandler(int channel, FifoDatamsgHandlerFunc handler, void* userdata);
void DSFifoSetValue32Handler(int channel, FifoValue32HandlerFunc handler, void* userdata);
void DSFifoSetAddressHandler(int channel, FifoAddressHandlerFunc handler, void* userdata);
word_t DSFifoGetRetValue32(int);
VoidFn DSIrqSet(word_t mask, VoidFn fn);
word_t DSProcessIRQs();
void DSWaitForIRQ(word_t mask);
void DSWaitForIRQRaw(word_t mask);
word_t DSWaitForNextIRQRaw();
word_t DSWaitForNextIRQ();

static inline void DSWaitForVBlank()
{
	DSWaitForIRQ(IRQ_VBLANK);
}

static inline void DSWaitForVBlankRaw()
{
	DSWaitForIRQRaw(IRQ_VBLANK);
}

int __getMode();
#define isUserMode() (__getMode() == 0x10)

enum { MODE_GET = -1, MODE_DISABLED = 0, MODE_NORMAL, MODE_HEADPHONES, MODE_COUNT };
int DSSetSuspendMode(int mode);

#define ERRSTR_INVALIDPARAM "Invalid parameter"
#define CHK_HMOD(hMod) sassert(AddressCheckMainRAM(hMod), ERRSTR_INVALIDPARAM);

#define tell(fd) lseek(fd, 0, SEEK_CUR)

typedef int (* hwreqfunc_t)(threadEP_t func, void* userData);
int DSRequestHardware(threadEP_t func, void* userData, hwreqfunc_t reqFunc);

void DSSetValue32Handler(int, bool);
void DSSetAddressHandler(int, bool);

typedef struct
{
	word_t refcount;
	module_t curModule;
	word_t* exitBuf;
	bool exitDone;
	void *stdin_hook;
	void *stdout_hook;
	void *stderr_hook;
#ifdef LIBFAT_FEOS_MULTICWD
	u32 cwdCluster;
	char* cwdBuffer;
#endif
} executeStatus_t;

extern executeStatus_t* curExecStatus;
typedef executeStatus_t* execstat_t;

execstat_t KeExecStatusCreate();
void KeExecStatusAddRef(execstat_t hSt);
void KeExecStatusRelease(execstat_t hSt);
void KeSetCurExecStatus(execstat_t hSt);
execstat_t KeGetCurExecStatus();

#ifdef LIBFAT_FEOS_MULTICWD
char* _getCwdBuf();
void _setCwdBuf(char* newBuf);

extern vu32* g_fatCwdClusterPtr;
#define g_fatCwdCluster (*g_fatCwdClusterPtr)

// Hidden libfat function that gets a pointer to the cwd cluster variable
uint32_t* _FAT_getCwdClusterPtr(const char* name);

void KeInitDefaultExecStatus();
#endif

#endif
