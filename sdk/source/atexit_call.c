#include "atexit_intl.h"

typedef struct tag_atexit_list
{
	atexitCb callback;
	void* obj;
	struct tag_atexit_list* next;
} atexit_list_t;

static atexit_list_t* atexitList;

int __reg_atexit(atexitCb func, void* obj)
{
	atexit_list_t* entry = (atexit_list_t*) malloc(sizeof(atexit_list_t));
	if (!entry) return 1;

	entry->callback = func;
	entry->obj = obj;
	entry->next = atexitList;
	atexitList = entry;
	return 0;
}

void __call_atexit()
{
	atexit_list_t* cur = atexitList;
	while (cur)
	{
		cur->callback(cur->obj);
		atexit_list_t* next = cur->next;
		free(cur);
		cur = next;
	}
}
