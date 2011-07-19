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

#define FXE2_MAGIC "FZ01"

enum
{
	FX2_M_SUBSYSTEM    = 0xF,
	FX2_SUBSYSTEM_EXEC = 1,
	FX2_SUBSYSTEM_DLIB = 2,
	FX2_SUBSYSTEM_ARM7 = 3,

	FX2_M_LDRFLAGS          = 0xFFF0,
	FX2_LDRFLAGS_COMPRESSED = 0x0010,

	FX2_IMP_SELECT_MODULE = 0xFFFFFFFF
};

#define FX2_SUBSYSTEM(n) ((n) & FX2_M_SUBSYSTEM)
#define FX2_LDRFLAGS(n) ((n) & FX2_M_LDRFLAGS)

#define FEOS_EP_INIT 0x00000001
#define FEOS_EP_FINI 0x00000002
#define FEOS_EP_MAIN 0x00000100

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

typedef struct tag_fxe_runtime_header
{
	instance_t hThis;
	const char* name;
	int refcount;
	int file;
	FeOSMain entrypoint;
	fxe_inmem_exports exp;
	fxe_inmem_imports imp;
	struct tag_fxe_runtime_header* next;
	struct tag_fxe_runtime_header* prev;
} fxe_runtime_header;

static inline fxe_runtime_header* GetRuntimeData(instance_t hinst)
{
	return (fxe_runtime_header*) *(word_t*) hinst;
}

instance_t LoadModule(const char* aFilename);
void FreeModule(instance_t hInst);

int ResolveImports(fxe2_import_t* imptbl, int count);
void FreeImports(fxe2_import_t* imptbl, int count);
void* FindInTbl(const fxe_inmem_exports* exphdr, const char* name);

void FeOS_ModuleListAdd(fxe_runtime_header* pModule);
void FeOS_ModuleListRemove(fxe_runtime_header* pModule);
int FeOS_ModuleListCount();
fxe_runtime_header* FeOS_ModuleListFind(const char* name);
