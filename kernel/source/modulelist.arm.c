#include "loader.h"

#define LINK_FAKEMODULE(NAME) \
	extern fxe_runtime_header _header_##NAME; \
	LdrModuleListAdd(&_header_##NAME)

extern fxe_runtime_header _header_FEOSKRNL;

static fxe_runtime_header* mListHead = &_header_FEOSKRNL;
static fxe_runtime_header* mListTail = &_header_FEOSKRNL;
static int nmodules = 1;

void LdrModuleListInit()
{
	LINK_FAKEMODULE(FEOSDSHW);
}

void LdrModuleListAdd(fxe_runtime_header* pModule)
{
	mListTail->next = pModule;
	pModule->next = NULL;
	pModule->prev = mListTail;
	mListTail = pModule;
	nmodules ++;
}

void LdrModuleListRemove(fxe_runtime_header* pModule)
{
	if (pModule->file == -1) return; // thwart attempts at doing evil

	pModule->prev->next = pModule->next;
	if (pModule->next) pModule->next->prev = pModule->prev;
	if (mListTail == pModule) mListTail = pModule->prev;
	nmodules --;
}

int LdrModuleListCount()
{
	return nmodules;
}

fxe_runtime_header* LdrModuleListFind(const char* name)
{
	fxe_runtime_header* item;
	for (item = mListHead; item != NULL; item = item->next)
		if (strcasecmp(name, item->name) == 0) return item;
	return NULL;
}

extern const byte_t __text_start[];
extern const byte_t __end__[];

void* LdrResolveAddr(void* addr)
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

void LdrEnumModules(moduleEnumCb cb, void* user_data)
{
	sassert(cb, ERRSTR_INVALIDPARAM);

	fxe_runtime_header* item;
	for (item = mListHead; item != NULL; item = item->next)
	{
		if (!cb(item->hThis, item->name, item->size, user_data))
			break;
	}
}
