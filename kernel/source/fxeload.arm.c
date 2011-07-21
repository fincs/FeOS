#include "fxe.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

static instance_t _LoadModule_imp(const char* aFilename, const char* aModuleName);

instance_t LoadModule(const char* aFilename)
{
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
			char buf[PATH_MAX+1];
			struct stat st;

			// First look at the lib folder
			snprintf(buf, sizeof(buf), "/data/FeOS/lib/%s.fx2", aFilename);
			if (stat(buf, &st) == 0) goto __LM_win;

			// Then at the bin folder
			snprintf(buf, sizeof(buf), "/data/FeOS/bin/%s.fx2", aFilename);
			if (stat(buf, &st) == 0) goto __LM_win;

			// Fail; we can't find the module
			return NULL;

__LM_win:
			return _LoadModule_imp(buf, aFilename);
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
		|| head.magic != 0x31305A46 /* FX01 */)
	{
		close(fd);
		return NULL;
	}

	totalsize = head.loadsize + head.bsssize + sizeof(fxe_runtime_header) + aModuleNameLen;
	if(totalsize > 0x200000)
	{
		close(fd);
		return NULL;
	}

	void* pMem = malloc(totalsize);
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

	// Write the pointer to the runtime header
	*(volatile word_t*)pMem = (word_t) rh;

	// Update caches
	FeOS_swi_DataCacheFlush(pMem, totalsize);
	FeOS_swi_InstrCacheInvalidate(pMem, totalsize);

	// Add this module to the list of loaded modules
	FeOS_ModuleListAdd(rh);

	// Run the constructors
	rh->entrypoint(FEOS_EP_INIT, 0, 0, 0);

	return pMem;
}

void FreeModule(instance_t hInst)
{
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
