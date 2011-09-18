#include <stdlib.h>
#include <string.h>
#include "feos.h"

static u8 fifoChns[FIFO_PROG_CH_NUM];

static int allocFifoChannel()
{
	register int i;
	for (i = 0; i < FIFO_PROG_CH_NUM; i ++)
		if (!fifoChns[i])
		{
			fifoChns[i] = 1;
			return FIFO_PROG_CH + i;
		}
	return -1;
}

static void freeFifoChannel(int ch)
{
	fifoChns[ch - FIFO_PROG_CH] = 0;
}

static void FeOSFifoHandler(int, void*);

void installFeOSFIFO()
{
	fifoSetDatamsgHandler(FIFO_FEOS, FeOSFifoHandler, NULL);
}

static volatile FeOSLoadStruct* ldSt = NULL;
static volatile instance_t freeMod = NULL;

void FeOSFifoHandler(int size, void* userdata)
{
	FeOSFifoMsg msg;
	fifoGetDatamsg(FIFO_FEOS, size, (u8*) &msg);

	switch(msg.type)
	{
		case FEOS_ARM7_LOAD_MODULE:
		{
			ldSt = msg.loadStruct;
			// Let the next VBlank process this
			break;
		}

		case FEOS_ARM7_UNLOAD_MODULE:
		{
			freeMod = msg.hModule;
			freeFifoChannel(msg.fifoCh);
			break;
		}
	}
}

static void FeOS_LoadModule();

void FeOS_VBlankFunc()
{
	if (ldSt)
	{
		FeOS_LoadModule();
		ldSt = NULL;
	}
	if (freeMod)
	{
		(*(FeOSMain*)freeMod)(FEOS_EP_FINI, 0, 0, 0);
		free(freeMod);
		freeMod = NULL;
		fifoSendValue32(FIFO_FEOS, 0);
	}
}

static bool FeOS_ResolveImp(word_t* addr, const char* name);

void FeOS_LoadModule()
{
	FeOSFifoMsg msg;
	msg.type = FEOS_ARM7_RETURN_MODULE;
	msg.hModule = NULL;
	msg.fifoCh = allocFifoChannel();
	if (msg.fifoCh == -1) goto _return;

	void* hModule = malloc(ldSt->size + ldSt->bsssize);
	if (!hModule)
	{
		freeFifoChannel(msg.fifoCh);
		goto _return;
	}

	memcpy(hModule, ldSt->data, ldSt->size);
	memset((u8*)hModule + ldSt->size, 0, ldSt->bsssize);

	// Apply relocations
	{
		register int i, j;
		word_t* patch = (word_t*) hModule;
		*patch += (word_t) hModule; // patch entrypoint
		for(i = 0; i < ldSt->nrelocs; i ++)
		{
			patch += ldSt->relocs[i].skip;
			for(j = 0; j < ldSt->relocs[i].patch; j ++)
				*patch++ += (word_t) hModule;
		}
	}

	// Resolve imports
	{
		register int i;
		fxe2_import_t* imps = ldSt->imps.table;
		for(i = 0; i < ldSt->imps.count; i ++)
		{
			if (imps[i].address == FX2_IMP_SELECT_MODULE)
				continue;

			word_t* addr = (word_t*)(imps[i].addr + (word_t)hModule);
			const char* name = (const char*)(imps[i].nameoffset + (word_t) imps);

			if (!FeOS_ResolveImp(addr, name))
			{
				free(hModule);
				freeFifoChannel(msg.fifoCh);
				goto _return;
			}
		}
	}

	if ((*(FeOSMain*)hModule)(FEOS_EP_INIT, msg.fifoCh, 0, 0) == FEOS_RC_OK)
		msg.hModule = hModule;
	else
	{
		free(hModule);
		freeFifoChannel(msg.fifoCh);
	}

_return:
	fifoSendDatamsg(FIFO_FEOS, sizeof(FeOSFifoMsg), (void*) &msg);
}

static void _infloop() { for(;;); }

bool FeOS_ResolveImp(word_t* addr, const char* name)
{
	*addr = (word_t) _infloop;
	return true;
}
