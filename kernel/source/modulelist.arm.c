#include "fxe.h"

#define LINK_FAKEMODULE(NAME) \
	extern fxe_runtime_header _header_##NAME; \
	FeOS_ModuleListAdd(&_header_##NAME)

extern fxe_runtime_header _header_FEOSBASE;

static fxe_runtime_header* mListHead = &_header_FEOSBASE;
static fxe_runtime_header* mListTail = &_header_FEOSBASE;
static int nmodules = 1;

void FeOS_ModuleListInit()
{
	LINK_FAKEMODULE(FEOSSTDIO);
	LINK_FAKEMODULE(FEOSPOSIXEMU);
	LINK_FAKEMODULE(FEOSMATH);
	LINK_FAKEMODULE(FEOSDSAPI);
	LINK_FAKEMODULE(FEOSDSSPR);
	LINK_FAKEMODULE(FEOSDSBG);
	LINK_FAKEMODULE(FEOSARM7);
}

void FeOS_ModuleListAdd(fxe_runtime_header* pModule)
{
	mListTail->next = pModule;
	pModule->next = NULL;
	pModule->prev = mListTail;
	mListTail = pModule;
	nmodules ++;
}

void FeOS_ModuleListRemove(fxe_runtime_header* pModule)
{
	if (pModule->file == -1) return; // thwart attempts at doing evil

	pModule->prev->next = pModule->next;
	if (pModule->next) pModule->next->prev = pModule->prev;
	if (mListTail == pModule) mListTail = pModule->prev;
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

extern const byte_t __text_start[];
extern const byte_t __end__[];

void* FeOS_ModuleFromAddress(void* addr)
{
	word_t addrw = (word_t) addr;

	if (addrw >= (word_t)__text_start && addrw < (word_t)__end__)
		return (void*)(~0);

	fxe_runtime_header* item;
	for (item = mListHead; item != NULL; item = item->next)
	{
		if (item->file == -1) continue; // Ignore fake modules

		word_t m_bottom = (word_t) item->hThis;
		word_t m_top = m_bottom + item->size;

		if (addrw >= m_bottom && addrw <= m_top)
			return item->hThis;
	}

	return NULL;
}

void FeOS_EnumModules(moduleEnumCb cb, void* user_data)
{
	fxe_runtime_header* item;
	for (item = mListHead; item != NULL; item = item->next)
	{
		if (!cb(item->hThis, item->name, item->size, user_data))
			break;
	}
}
