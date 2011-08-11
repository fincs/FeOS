#pragma once
#include <nds.h>

#define ITCM_DATA __attribute__((section(".itcm")))

#define FeOS_AllocStack(a) __builtin_alloca(a)

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
		NULL, /* entrypoint */ \
		MAKE_EXPORTSTRUCT(_NAME_), /* exp */ \
		{ 0, NULL }, /* imp */ \
		{ NULL, 0 }, /* exidx */ \
		NULL, /* next */ \
		NULL /* prev */ \
	}; \
	\
	void* _inst_##_NAME_ = &_header_##_NAME_;

typedef unsigned int word_t;
typedef unsigned short hword_t;
typedef unsigned char byte_t;
typedef word_t bool_t;
typedef void* instance_t;

void DoTheUserMode();
void PrepareUserMode();

static inline bool AddressCheckMainRAM(const void* addr)
{
	return (word_t)addr >= 0x02000000 && (word_t)addr <= 0x03000000;
}

#define E_FILENOTFOUND (-10)
#define E_INVALIDARG (-11)
#define E_APPKILLED (-12)

int FeOS_IsValidName(const char* path);

void FeOS_WaitForVBlank();

void FeOS_swi_DataCacheFlush(const void* mem, size_t size);
void FeOS_swi_DataCacheFlush();
void FeOS_swi_InstrCacheInvalidate(const void* mem, size_t size);
void FeOS_swi_InstrCacheInvalidateAll();
