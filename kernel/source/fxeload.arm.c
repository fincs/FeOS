#include "fxe.h"
#include "feosfifo.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

static instance_t _LoadModule_imp(const char* aFilename, const char* aModuleName);

static const char* moduleSearchPaths[] =
{
	"/data/FeOS/lib",
	"/data/FeOS/bin",
	NULL
};

instance_t LoadModule(const char* aFilename)
{
	sassert(aFilename, ERRSTR_INVALIDPARAM);
	if (*aFilename == '/')
	{
		// Path was specified, we have to extract the module name.

		int name_len = strlen(aFilename);
		int ext_pos = name_len;
		int i;
		for (i = name_len-1; i >= 0; i --)
		{
			if (aFilename[i] == '.' && ext_pos == name_len)
			{
				ext_pos = i;
				continue;
			}
			if (aFilename[i] == '/')
			{
				i ++;
				break;
			}
		}
		if (!i) return NULL;

		name_len = ext_pos - i;

		char* aModuleName = (char*) malloc(name_len+1);
		memcpy(aModuleName, aFilename + i, name_len);
		aModuleName[name_len] = '\0';

		// Check if the module is already loaded
		{
			fxe_runtime_header* header = FeOS_ModuleListFind(aModuleName);
			if (header)
			{
				header->refcount ++;
				free(aModuleName);
				return header->hThis;
			}
		}

		instance_t ret = _LoadModule_imp(aFilename, aModuleName);
		free(aModuleName);
		return ret;
	}else
	{
		// Check if the module is already loaded
		{
			fxe_runtime_header* header = FeOS_ModuleListFind(aFilename);
			if (header)
			{
				header->refcount ++;
				return header->hThis;
			}
		}

		// The module name was specified, we have to find the path to the module itself
		{
			char* buf = (char*) malloc(PATH_MAX+1);
			if (!buf) return NULL;

			struct stat st;

			// Iterate through the search paths in order to find the module
			const char** aBuf;
			for (aBuf = moduleSearchPaths; *aBuf; aBuf ++)
			{
				sniprintf(buf, PATH_MAX+1, "%s/%s.fx2", *aBuf, aFilename);
				if (stat(buf, &st) == 0)
					break;
			}

			// Load the module
			instance_t ret = *aBuf ? _LoadModule_imp(buf, aFilename) : NULL;
			free(buf);
			return ret;
		}
	}
}

static instance_t _LoadModule_imp(const char* aFilename, const char* aModuleName)
{
	int aModuleNameLen = strlen(aModuleName)+1;

	int fd = open(aFilename, O_RDONLY);
	if(fd == -1)
		return NULL;

	fxe2_header_t head;
	size_t totalsize;
	if(read(fd, &head, sizeof(fxe2_header_t)) != sizeof(fxe2_header_t)
		|| head.magic != 0x31305A46 /* FX01 */
		|| FX2_SUBSYSTEM(head.flags) != FX2_SUBSYSTEM_STANDARD
		|| FEOS_VPACK_MAKE(head.osmajorver, head.osminorver) > FEOS_VERSION_PACK)
	{
		close(fd);
		return NULL;
	}

	totalsize = head.loadsize + head.bsssize + sizeof(fxe_runtime_header) + aModuleNameLen;

	void* pMem = memalign(32, (totalsize+31) & ~31);
	if(pMem == NULL)
	{
		close(fd);
		return NULL;
	}

	if(read(fd, pMem, head.loadsize) != head.loadsize)
	{
		free(pMem);
		close(fd);
		return NULL;
	}

	// Clear the BSS
	//dmaFillWords(0, pMem + head.loadsize, head.bsssize);
	memset(pMem + head.loadsize, 0, head.bsssize);

	if(head.nrelocs)
	{
		word_t relocsize = head.nrelocs * sizeof(fxe2_reloc_t);
		fxe2_reloc_t* pRelocs = (fxe2_reloc_t*) malloc(relocsize);
		if(!pRelocs)
		{
			free(pMem);
			close(fd);
			return NULL;
		}

		if(read(fd, pRelocs, relocsize) != relocsize)
		{
			free(pRelocs);
			free(pMem);
			close(fd);
			return NULL;
		}

		// TODO: range checks
		register int i, j;
		word_t* patch = (word_t*) pMem;
		for(i = 0; i < head.nrelocs; i ++)
		{
			patch += pRelocs[i].skip;
			for(j = 0; j < pRelocs[i].patch; j ++)
				*patch++ += (word_t) pMem;
		}

		free(pRelocs);
	}

	// Build the runtime header
	char* namebuf = (char*)(pMem + totalsize - aModuleNameLen);
	fxe_runtime_header* rh = (fxe_runtime_header*)(namebuf - sizeof(fxe_runtime_header));
	rh->hThis = pMem;
	rh->name = namebuf;
	rh->refcount = 1;
	rh->file = fd;
	rh->size = head.loadsize + head.bsssize;
	rh->entrypoint = (FeOSMain) (pMem + head.entrypoint);
	rh->exp.count = head.nexports;
	rh->imp.count = head.nimports;

	strcpy(namebuf, aModuleName);

	// Read exports
	if (head.nexports && head.sexports)
	{
		word_t sexports = head.sexports;
		word_t nexports = head.nexports;
		
		fxe2_export_t* exptbl = (fxe2_export_t*) malloc(sexports);
		if (!exptbl)
		{
			free(pMem), close(fd);
			return NULL;
		}

		if (read(fd, exptbl, sexports) != sexports)
		{
			free(exptbl), free(pMem), close(fd);
			return NULL;
		}

		register int i;
		for(i = 0; i < nexports; i ++)
		{
			// TODO: range checks
			exptbl[i].nameoffset += (word_t) exptbl;
			exptbl[i].address += (word_t) pMem;
		}

		rh->exp.table = exptbl;
	}

	// Read imports
	if (head.nimports && head.simports)
	{
		/* It's... basically the same code!!!111 */
		word_t simports = head.simports;
		word_t nimports = head.nimports;

		fxe2_import_t* imptbl = (fxe2_import_t*) malloc(simports);
		if (!imptbl)
		{
			free(pMem), close(fd);
			return NULL;
		}

		if (read(fd, imptbl, simports) != simports)
		{
			free(imptbl), free(pMem), close(fd);
			return NULL;
		}

		register int i;
		for(i = 0; i < nimports; i ++)
		{
			// TODO: range checks
			imptbl[i].nameoffset += (word_t) imptbl;
			if (imptbl[i].address != FX2_IMP_SELECT_MODULE)
				imptbl[i].address += (word_t) pMem;
		}

		if (!ResolveImports(imptbl, nimports))
		{
			free(imptbl), free(pMem), close(fd);
			return NULL;
		}

		rh->imp.table = imptbl;
	}

	if(head.flags & FX2_LDRFLAGS_HASIMPCOPY)
	{
		fxe2_impcopy_head_t ich;
		fxe2_impcopy_t impcpy;

		if(read(fd, &ich, sizeof(ich)) != sizeof(ich))
		{
_impcopy_err:
			free(pMem);
			close(fd);
			return NULL;
		}

		if(ich.type != FX2_IMPCOPY_NORMAL)
			goto _impcopy_err;

		// TODO: range checks
		register int i;
		for(i = 0; i < ich.count; i ++)
		{
			if(read(fd, &impcpy, sizeof(impcpy)) != sizeof(impcpy))
				goto _impcopy_err;

			impcpy.from += (word_t) pMem;
			impcpy.to += (word_t) pMem;

			word_t impcpy_data = *impcpy.pTo;
			word_t impcpy_off = (word_t)((int)impcpy_data >> 4);

			switch (impcpy_data & 0xF)
			{
				case 0: // simple copy
					*impcpy.pTo = *impcpy.pFrom + impcpy_off;
					break;

				case 1: // relative offset
					*impcpy.pTo = *impcpy.pFrom - impcpy.to + impcpy_off;
					break;

				default: goto _impcopy_err;
			}
		}
	}

	// Write the pointer to the runtime header
	*(volatile word_t*)pMem = (word_t) rh;

	// Update caches
	FeOS_swi_DataCacheFlush(pMem, totalsize);
	FeOS_swi_InstrCacheInvalidate(pMem, totalsize);

	// Add this module to the list of loaded modules
	FeOS_ModuleListAdd(rh);

	// Get the start and size of the extra data section
	rh->extrapos = tell(fd);
	lseek(fd, 0, SEEK_END);
	rh->extrasize = tell(fd) - rh->extrapos;
	lseek(fd, rh->extrapos, SEEK_SET);

	// Run the constructors
	rh->entrypoint(FEOS_EP_INIT, 0, 0, 0);

	// Get the exception index table
	if (rh->entrypoint(FEOS_EP_GETEXIDXTBL, (word_t) &rh->exidx, 0, 0) != FEOS_RC_OK)
	{
		rh->exidx.table = NULL;
		rh->exidx.nentries = 0;
	}

	return pMem;
}

int ModuleExtraGetSize(instance_t hInst)
{
	CHK_HINST(hInst);
	fxe_runtime_header* rh = GetRuntimeData(hInst);
	return rh->extrasize;
}

int ModuleExtraRead(instance_t hInst, void* buf, size_t size)
{
	CHK_HINST(hInst);
	fxe_runtime_header* rh = GetRuntimeData(hInst);
	return read(rh->file, buf, size);
}

int ModuleExtraSeek(instance_t hInst, int pos, int mode)
{
	CHK_HINST(hInst);
	fxe_runtime_header* rh = GetRuntimeData(hInst);
	int fd = rh->file;
	int base = rh->extrapos;
	switch (mode)
	{
		case SEEK_SET:
			return lseek(fd, base+pos, SEEK_SET);
		case SEEK_CUR:
		{
			int target = tell(fd) + pos;
			if (target < base) target = base;
			return lseek(fd, target, SEEK_SET);
		}
		case SEEK_END:
		{
			int target = base + rh->extrasize + pos;
			if (target < base) target = base;
			return lseek(fd, target, SEEK_SET);
		}
	}
	return -1;
}

int ModuleExtraTell(instance_t hInst)
{
	CHK_HINST(hInst);
	fxe_runtime_header* rh = GetRuntimeData(hInst);
	return tell(rh->file) - rh->extrapos;
}

static FeOSLoadStruct __ldSt;

//#define GET_LOADST() ((FeOSLoadStruct*)memUncached(&__ldSt))
#define GET_LOADST() (&__ldSt)

instance_t LoadModule_ARM7(const char* aFilename, int* pFifoCh)
{
	sassert(aFilename, ERRSTR_INVALIDPARAM);
	sassert(pFifoCh, ERRSTR_INVALIDPARAM);

	FeOSLoadStruct* ldSt = GET_LOADST();
	FeOSFifoMsg msg;
	msg.type = FEOS_ARM7_LOAD_MODULE;
	msg.loadStruct = ldSt;

	int fd = open(aFilename, O_RDONLY);
	if(fd == -1)
		return NULL;

	fxe2_header_t head;
	if(read(fd, &head, sizeof(fxe2_header_t)) != sizeof(fxe2_header_t)
		|| head.magic != 0x31305A46 /* FX01 */)
	{
_shorterr:
		close(fd);
		return NULL;
	}

	size_t readsize;
	void* pMem = malloc((readsize = head.loadsize + head.nrelocs*sizeof(fxe2_reloc_t)) + head.simports);
	if(pMem == NULL) goto _shorterr;

	// Read loadable section + relocations
	if(read(fd, pMem, readsize) != readsize)
	{
_fullerr:
		free(pMem);
		goto _shorterr;
	}

	// Skip over exports
	lseek(fd, head.sexports, SEEK_CUR);

	// Read imports
	if(read(fd, (u8*)pMem + readsize, head.simports) != head.simports)
		goto _fullerr;

	close(fd);

	// Set the entrypoint
	*(volatile word_t*)pMem = head.entrypoint;

	// Fill in loadStruct structure
	ldSt->data = pMem;
	ldSt->size = head.loadsize;
	ldSt->bsssize = head.bsssize;
	ldSt->imps.count = head.nimports;
	ldSt->imps.table = (fxe2_import_t*)((u8*)pMem + readsize);
	ldSt->nrelocs = head.nrelocs;
	ldSt->relocs = (fxe2_reloc_t*)((u8*)pMem + head.loadsize);

	DC_FlushRange(pMem, readsize + head.simports);
	DC_FlushRange(ldSt, sizeof(FeOSLoadStruct));

	// Tell the ARM7 to load the module
	fifoSendDatamsg(FIFO_FEOS, sizeof(FeOSFifoMsg), (void*) &msg);

	// Wait for it to load
	while(!fifoCheckDatamsg(FIFO_FEOS));

	// Free the temporary memory
	free(pMem);

	// Return
	fifoGetDatamsg(FIFO_FEOS, sizeof(FeOSFifoMsg), (void*) &msg);
	*pFifoCh = msg.fifoCh;
	return msg.hModule;
}

void FreeModule_ARM7(instance_t hModule, int fifoCh)
{
	sassert(hModule, ERRSTR_INVALIDPARAM);

	FeOSFifoMsg msg;
	msg.type = FEOS_ARM7_UNLOAD_MODULE;
	msg.hModule = hModule;
	msg.fifoCh = fifoCh;
	fifoSendDatamsg(FIFO_FEOS, sizeof(FeOSFifoMsg), (void*) &msg);
	while(!fifoCheckValue32(FIFO_FEOS));
	fifoGetValue32(FIFO_FEOS);
}

void FreeModule(instance_t hInst)
{
	CHK_HINST(hInst);

	fxe_runtime_header* rh = GetRuntimeData(hInst);

	rh->refcount --;
	if (rh->refcount) return;

	// Run the destructors
	rh->entrypoint(FEOS_EP_FINI, 0, 0, 0);

	// Remove the module from the list of loaded modules
	FeOS_ModuleListRemove(rh);

	// Free the export table
	if (rh->exp.count)
		free(rh->exp.table);

	// Free the import table
	if (rh->imp.count)
	{
		FreeImports(rh->imp.table, rh->imp.count);
		free(rh->imp.table);
	}

	// Close the file
	close(rh->file);

	// Free the module memory
	free(hInst);
}

void ModuleLock(instance_t hInst)
{
	CHK_HINST(hInst);
	GetRuntimeData(hInst)->refcount ++;
}

void ModuleUnlock(instance_t hInst)
{
	CHK_HINST(hInst);
	GetRuntimeData(hInst)->refcount --;
}

extern const fxe_inmem_exports _exp_FEOSBASE;

void* FindInTbl(const fxe_inmem_exports* exphdr, const char* name)
{
	register int i;
	for(i = 0; i < exphdr->count; i ++)
		if (strcmp(exphdr->table[i].name, name) == 0)
			return exphdr->table[i].addr;
	return NULL;
}

int ResolveImports(fxe2_import_t* imptbl, int count)
{
	register int i;
	fxe_inmem_exports* exptable = NULL;

	for(i = 0; i < count; i ++)
	{
		fxe2_import_t* imp = imptbl + i;
		void* sym = NULL;

		if(imp->address == FX2_IMP_SELECT_MODULE)
		{
			instance_t hModule = LoadModule(imp->name);
			if (!hModule) return 0;

			exptable = &GetRuntimeData(hModule)->exp;
			continue;
		}

		if (!exptable) return 0;

		sym = FindInTbl(exptable, imp->name);

		if (!sym) return 0;

		*(imp->addr) = sym;
	}
	return 1;
}

void FreeImports(fxe2_import_t* imptbl, int count)
{
	register int i;

	for(i = 0; i < count; i ++)
	{
		fxe2_import_t* imp = imptbl + i;

		if(imp->address != FX2_IMP_SELECT_MODULE) continue;

		fxe_runtime_header* header = FeOS_ModuleListFind(imp->name);
		if (!header) continue; // oops, memory corruption? (shouldn't happen)

		FreeModule(header->hThis);
	}
}
