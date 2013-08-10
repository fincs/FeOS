#pragma once
#include "feos.h"

typedef struct
{
	word_t magic; // FZ01
	word_t loadsize;
	word_t bsssize;
	word_t nrelocs;

	byte_t osmajorver;
	byte_t osminorver;
	hword_t flags;
	word_t entrypoint;
	hword_t nexports;
	hword_t sexports;
	hword_t nimports;
	hword_t simports;
} fxe2_header_t;

typedef struct
{
	hword_t skip;
	hword_t patch;
} fxe2_reloc_t;

typedef struct
{
	union
	{
		word_t nameoffset;
		const char* name;
	};
	union
	{
		word_t address;
		void* addr;
	};
} fxe2_export_t;

typedef struct
{
	union
	{
		word_t nameoffset;
		const char* name;
	};
	union
	{
		word_t address;
		void** addr;
	};
} fxe2_import_t;

typedef struct
{
	word_t type;
	word_t count;
} fxe2_impcopy_head_t;

typedef struct
{
	union
	{
		word_t from;
		volatile word_t* pFrom;
	};
	union
	{
		word_t to;
		volatile word_t* pTo;
	};
} fxe2_impcopy_t;

#define FXE2_MAGIC "FZ01"

enum
{
	FX2_M_SUBSYSTEM    = 0xF,
	FX2_SUBSYSTEM_INVALID  = 0,
	FX2_SUBSYSTEM_STANDARD = 1,

	FX2_M_LDRFLAGS          = 0xFFF0,
	FX2_LDRFLAGS_COMPRESSED = 0x0010,
	FX2_LDRFLAGS_HASIMPCOPY = 0x0020,

	FX2_IMP_SELECT_MODULE = 0xFFFFFFFF,

	FX2_IMPCOPY_NORMAL = 0
};

#define FX2_SUBSYSTEM(n) ((n) & FX2_M_SUBSYSTEM)
#define FX2_LDRFLAGS(n) ((n) & FX2_M_LDRFLAGS)

#define FEOS_EP_INIT 0x00000001
#define FEOS_EP_FINI 0x00000002
#define FEOS_EP_GETEXIDXTBL 0x00000003
#define FEOS_EP_MAIN 0x00000100

typedef struct
{
	void* table;
	int nentries;
} FeOS_GetExidxTbl_t;

#define FEOS_RC_OK 0
#define FEOS_RC_ERR 0xFFFFFFFF

typedef word_t (* FeOSMain)(word_t, word_t, word_t, word_t);

typedef struct
{
	int count;
	fxe2_export_t* table;
} fxe_inmem_exports;

typedef struct
{
	int count;
	fxe2_import_t* table;
} fxe_inmem_imports;

#ifdef ARM9

typedef struct tag_fxe_runtime_header
{
	module_t hThis;
	const char* name;
	int refcount;
	int file;
	size_t size;
	int extrapos, extrasize;
	FeOSMain entrypoint;
	fxe_inmem_exports exp;
	fxe_inmem_imports imp;
	FeOS_GetExidxTbl_t exidx;
	struct tag_fxe_runtime_header* next;
	struct tag_fxe_runtime_header* prev;
} fxe_runtime_header;

static inline fxe_runtime_header* GetRuntimeData(module_t hMod)
{
	return (fxe_runtime_header*) *(word_t*) hMod;
}

module_t LdrLoadModule(const char* aFilename);
void LdrFreeModule(module_t hMod);
module_t LdrGetModule(const char* name);
const char* LdrGetModuleName(module_t hMod);

void* LdrFindSymbol(module_t hMod, const char* sym);

void LdrLockModule(module_t hMod);
void LdrUnlockModule(module_t hMod);

int LdrResGetSize(module_t hMod);
int LdrResRead(module_t hMod, void* buf, size_t size);
int LdrResSeek(module_t hMod, int pos, int mode);
int LdrResTell(module_t hMod);

int LdrExecuteArgv(int, const char*[]);

int LdrResolveImports(fxe2_import_t* imptbl, int count);
void LdrFreeImports(fxe2_import_t* imptbl, int count);
void* LdrFindInTbl(const fxe_inmem_exports* exphdr, const char* name);

void LdrModuleListInit();
void LdrModuleListAdd(fxe_runtime_header* pModule);
void LdrModuleListRemove(fxe_runtime_header* pModule);
int LdrModuleListCount();
fxe_runtime_header* LdrModuleListFind(const char* name);

typedef int (* moduleEnumCb)(module_t hMod, const char* name, size_t size, void* user_data);

void LdrEnumModules(moduleEnumCb cb, void* user_data);
void LdrModuleExit(int);

void* LdrGetExidxTbl(module_t, int*);
void* LdrResolveAddr(void*);

#endif
