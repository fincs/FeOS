#include "fxe.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>

instance_t LoadModule(const char* aFilename)
{
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

	totalsize = head.loadsize + head.bsssize + sizeof(fxe_runtime_header);
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
	void* pMemUncached = memUncached(pMem);

	FeOS_swi_DataCacheFlush(pMem, totalsize);
	FeOS_swi_InstrCacheInvalidate(pMem, totalsize);

	if(read(fd, pMemUncached, head.loadsize) != head.loadsize)
	{
		free(pMem);
		close(fd);
		return NULL;
	}

	// Clear the BSS
	//dmaFillWords(0, pMemUncached + head.loadsize, head.bsssize);
	memset(pMemUncached + head.loadsize, 0, head.bsssize);

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
		word_t* patch = (word_t*) pMemUncached;
		for(i = 0; i < head.nrelocs; i ++)
		{
			patch += pRelocs[i].skip;
			for(j = 0; j < pRelocs[i].patch; j ++)
				*patch++ += (word_t) pMem;
		}

		free(pRelocs);
	}

	// Build the runtime header
	fxe_runtime_header* rh = (fxe_runtime_header*)(pMemUncached + totalsize - sizeof(fxe_runtime_header));
	rh->file = fd;
	rh->entrypoint = (FeOSMain) (pMem + head.entrypoint);

	// Read exports
	if (head.nexports && head.sexports)
	{
		word_t sexports = head.sexports;
		word_t nexports = head.nexports;
		
		rh->exp.count = nexports;
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

		rh->exp.table = exptbl; // That's it...
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
				imptbl[i].address += (word_t) pMemUncached;
		}

		if (!ResolveImports(imptbl, nimports))
		{
			free(imptbl), free(pMem), close(fd);
			return NULL;
		}

		free(imptbl);
	}

	// Write the pointer to the runtime header
	*(word_t*)pMemUncached = (word_t) rh;

	// Run the constructors
	rh->entrypoint(FEOS_EP_INIT, 0, 0, 0);

	return pMem;
}

void FreeModule(instance_t hInst)
{
	fxe_runtime_header* rh = GetRuntimeData(hInst);

	// Run the destructors
	rh->entrypoint(FEOS_EP_FINI, 0, 0, 0);

	// Free the export table
	if (rh->exp.count)
		free(rh->exp.table);

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
	for(i = 0; i < count; i ++)
	{
		fxe2_import_t* imp = imptbl + i;
		void* sym = NULL;

		// TEMPTEMPTEMP HACKHACKHACK
		if(imp->address == FX2_IMP_SELECT_MODULE)
			continue;

		// MORE TEMPTEMPTEMP HACKHACKHACK
		//if(strcmp(imp->name, "ImpFunc") == 0) sym = ImpFunc;
		sym = FindInTbl(&_exp_FEOSBASE, imp->name);

		if (!sym)
		{
			iprintf("Can't resolve sym: %s\n", imp->name);
			return 0;
		}

		*(imp->addr) = sym;
	}
	return 1;
}
