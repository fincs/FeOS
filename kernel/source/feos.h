#pragma once
#include <nds.h>
#include "../../sdk/include/feosver.h"

#ifdef ARM9
#define ITCM_DATA __attribute__((section(".itcm")))
#endif

#define FeOS_AllocStack(a) __builtin_alloca(a)

#ifdef ARM9
#define BEGIN_TABLE(_NAME_) static const fxe2_export_t _exp_##_NAME_##_tbl[] = {
#define ADD_FUNC(FUNC) {{(word_t)#FUNC}, {(word_t)FUNC}}
#define ADD_FUNC_ALIAS(FUNC, NAME) {{(word_t)#NAME}, {(word_t)FUNC}}
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
typedef void* instance_t;

#ifdef ARM9
#include "feos_setjmp.h"

void DoTheUserMode();
void PrepareUserMode();

word_t __ARMSWP(word_t value, volatile word_t* addr);

extern bool_t __inFAT;

static inline bool AddressCheckMainRAM(const void* addr)
{
	return (word_t)addr >= 0x02000000 && (word_t)addr <= 0x03000000;
}

#define E_FILENOTFOUND (-10)
#define E_INVALIDARG (-11)
#define E_APPKILLED (-12)

void FeOS_WaitForIRQ(word_t);
static inline void FeOS_WaitForVBlank()
{
	FeOS_WaitForIRQ(IRQ_VBLANK);
}

typedef void (*irqWaitFunc_t)(word_t);

void FeOS_RunFifoQueue();
void FeOS_FifoSetDatamsgHandler(int channel, FifoDatamsgHandlerFunc handler, void* userdata);
void FeOS_FifoSetValue32Handler(int channel, FifoValue32HandlerFunc handler, void* userdata);
void FeOS_FifoSetAddressHandler(int channel, FifoAddressHandlerFunc handler, void* userdata);
VoidFn FeOS_SetInterrupt(word_t mask, VoidFn fn);
word_t FeOS_CheckPendingIRQs();
void FeOS_WaitForIRQ(word_t mask);
word_t FeOS_NextIRQ();

#define GET_IRQFUNC ((irqWaitFunc_t)0xFFFFFFFF)
irqWaitFunc_t FeOS_SetIRQWaitFunc(irqWaitFunc_t newFunc);

int __getMode();
#define isUserMode() (__getMode() == 0x10)

enum { MODE_GET = -1, MODE_DISABLED = 0, MODE_NORMAL, MODE_HEADPHONES, MODE_COUNT };
int FeOS_swi_SetSuspendMode(int mode);

#define ERRSTR_INVALIDPARAM "Invalid parameter"
#define CHK_HINST(hInst) sassert(AddressCheckMainRAM(hInst), ERRSTR_INVALIDPARAM);

#define tell(fd) lseek(fd, 0, SEEK_CUR)

extern bool bOAMUpd, bBgUpd, bKeyUpd;

enum { AUTOUPD_OAM = 1, AUTOUPD_BG, AUTOUPD_KEYS };
void FeOS_SetAutoUpdate(int which, bool enable);
bool FeOS_GetAutoUpdate(int which);

void FeOS_swi_DataCacheFlush(const void* mem, size_t size);
void FeOS_swi_DataCacheFlushAll();
void FeOS_swi_InstrCacheInvalidate(const void* mem, size_t size);
void FeOS_swi_InstrCacheInvalidateAll();
void FeOS_swi_IrqEnable(word_t mask);
void FeOS_swi_IrqDisable(word_t mask);
//void FeOS_swi_SetDatamsgHandler(int, bool);
int FeOS_swi_FifoCheckDatamsgLength(int ch);
void FeOS_swi_SetValue32Handler(int, bool);
void FeOS_swi_SetAddressHandler(int, bool);
#endif
