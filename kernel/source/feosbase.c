#include "feos.h"
#include "fxe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

void FeOS_swi_DebugPrint(const char*);

void* FeOS_FindSymbol(instance_t hinst, const char* sym)
{
	if (!AddressCheckMainRAM((void*) hinst))
		return NULL;
	return FindInTbl(&GetRuntimeData(hinst)->exp, sym);
}

int __aeabi_idiv(int, int);
int __aeabi_idivmod(int, int);
unsigned int __aeabi_uidiv(unsigned int, unsigned int);
unsigned int __aeabi_uidivmod(unsigned int, unsigned int);

BEGIN_TABLE(FEOSBASE)
	ADD_FUNC_ALIAS(LoadModule, FeOS_LoadModule),
	ADD_FUNC(FeOS_FindSymbol),
	ADD_FUNC_ALIAS(FreeModule, FeOS_FreeModule),
	ADD_FUNC(FeOS_Execute),
	ADD_FUNC(FeOS_WaitForVBlank),
	ADD_FUNC_ALIAS(FeOS_swi_DebugPrint, FeOS_DebugPrint),
	ADD_FUNC_ALIAS(__errno, FeOS_GetErrnoPtr),
	ADD_FUNC(__aeabi_idiv),
	ADD_FUNC(__aeabi_idivmod),
	ADD_FUNC(__aeabi_uidiv),
	ADD_FUNC(__aeabi_uidivmod),

	// stdlib.h
	ADD_FUNC(atoi),
	ADD_FUNC(strtol),
	ADD_FUNC(strtoul),
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

int FeOS_Execute(int argc, const char* argv[])
{
	if (argc == 0) return E_INVALIDARG;
	instance_t hInst = LoadModule(argv[0]);
	if (!hInst) return E_FILENOTFOUND;

	fxe_runtime_header* rh = GetRuntimeData(hInst);

	int rc = E_INVALIDARG;
	if (rh->entrypoint) rc = rh->entrypoint(FEOS_EP_MAIN, (word_t)argc, (word_t)argv, 0);

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