#include "feos.h"
#include "fxe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BEGIN_TABLE(_NAME_) static const fxe2_export_t _exp_##_NAME_##_tbl[] = {
#define ADD_FUNC(FUNC) {{(word_t)#FUNC}, {(word_t)FUNC}}
#define ADD_FUNC_ALIAS(FUNC, NAME) {{(word_t)#NAME}, {(word_t)FUNC}}
#define END_TABLE(_NAME_) }; const fxe_inmem_exports _exp_##_NAME_ = { sizeof(_exp_##_NAME_##_tbl) / sizeof(fxe2_export_t), (fxe2_export_t*) _exp_##_NAME_##_tbl};

void FeOS_swi_DebugPrint(const char*);

void ImpFunc()
{
	FeOS_swi_DebugPrint("Hey, this actually WORKED!\n");
}

void* FeOS_FindSymbol(instance_t hinst, const char* sym)
{
	if (!AddressCheckMainRAM((void*) hinst))
		return NULL;
	return FindInTbl(&GetRuntimeData(hinst)->exp, sym);
}

BEGIN_TABLE(FEOSBASE)
	ADD_FUNC(ImpFunc),
	ADD_FUNC(FeOS_FindSymbol),
	ADD_FUNC(FeOS_WaitForVBlank),
	ADD_FUNC_ALIAS(FeOS_swi_DebugPrint, FeOS_DebugPrint),

	// stdlib.h
	ADD_FUNC(atoi),
	ADD_FUNC(rand),
	ADD_FUNC(srand),
	ADD_FUNC(malloc),
	ADD_FUNC(calloc),
	ADD_FUNC(free),
	ADD_FUNC(realloc),
	ADD_FUNC(bsearch),
	ADD_FUNC(qsort),
	ADD_FUNC(abs),
	ADD_FUNC(div), // to be replaced with native div hardware

	// string.h
	ADD_FUNC(strlen),
	ADD_FUNC(strcpy),
	ADD_FUNC(strcat),
	ADD_FUNC(strcmp),
	ADD_FUNC(strncpy),
	ADD_FUNC(strncat),
	ADD_FUNC(strncmp),
	ADD_FUNC(memcpy),
	ADD_FUNC(memmove),
	ADD_FUNC(memset),
	ADD_FUNC(memcmp),

	ADD_FUNC_ALIAS(iprintf, printf),
	ADD_FUNC_ALIAS(siprintf, sprintf)
END_TABLE(FEOSBASE)

/*
int FeOS_LoadExecutable(const char* path, const char* cmdline)
{
	instance_t inst;

	if(FeOS_IsValidName(path) < 0)
		return -1;

	inst = LoadExecutable(path);
	if(!inst)
		return -1;

	return GetRuntimeData(inst)->entrypoint(cmdline);
}
*/

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
