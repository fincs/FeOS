#include "feos.h"
#include "fxe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void FeOS_swi_DebugPrint(const char*);

void* FeOS_FindSymbol(instance_t hinst, const char* sym)
{
	if (!AddressCheckMainRAM((void*) hinst))
		return NULL;
	return FindInTbl(&GetRuntimeData(hinst)->exp, sym);
}

BEGIN_TABLE(FEOSBASE)
	ADD_FUNC_ALIAS(LoadModule, FeOS_LoadModule),
	ADD_FUNC(FeOS_FindSymbol),
	ADD_FUNC_ALIAS(FreeModule, FeOS_FreeModule),
	ADD_FUNC(FeOS_Execute),
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
	ADD_FUNC(memcmp)
END_TABLE(FEOSBASE)

extern void* _inst_FEOSBASE;

fxe_runtime_header _header_FEOSBASE =
{
	&_inst_FEOSBASE, // hThis
	"FEOSBASE", // name
	1, // refcount
	-1, // file
	NULL, // entrypoint
	MAKE_EXPORTSTRUCT(FEOSBASE), // exp
	{ 0, NULL }, // imp
	NULL, // next
	NULL // prev
};

void* _inst_FEOSBASE = &_header_FEOSBASE;


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

int FeOS_Execute(int argc, const char* argv[])
{
	if (argc == 0) return -1;
	instance_t hInst = LoadModule(argv[0]);
	if (!hInst) return -1;

	int rc = GetRuntimeData(hInst)->entrypoint(FEOS_EP_MAIN, (word_t)argc, (word_t)argv, 0);
	FreeModule(hInst);

	return rc;
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
