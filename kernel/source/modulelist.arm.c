#include "fxe.h"

extern fxe_runtime_header _header_FEOSBASE;

static fxe_runtime_header* mListHead = &_header_FEOSBASE;
static fxe_runtime_header* mListTail = &_header_FEOSBASE;
static int nmodules = 1;

void FeOS_ModuleListAdd(fxe_runtime_header* pModule)
{
	mListTail->next = pModule;
	pModule->prev = mListTail;
	mListTail = pModule;
	nmodules ++;
}

void FeOS_ModuleListRemove(fxe_runtime_header* pModule)
{
	if (pModule == mListHead) return; // thwart attempts at doing evil
	
	pModule->prev->next = pModule->next;
	nmodules --;
}

int FeOS_ModuleListCount()
{
	return nmodules;
}

fxe_runtime_header* FeOS_ModuleListFind(const char* name)
{
	fxe_runtime_header* item;
	for (item = mListHead; item != NULL; item = item->next)
		if (stricmp(name, item->name) == 0) return item;
	return NULL;
}
