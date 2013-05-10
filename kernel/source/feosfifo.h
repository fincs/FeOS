#pragma once
#include "feos.h"
#include "fxe.h"

#define FIFO_BASE_CH FIFO_USER_01
#define FIFO_FEOS (FIFO_USER_01)
#define FIFO_PROG_CH (FIFO_FEOS+1)
#define FIFO_PROG_CH_NUM 7

enum
{
	FEOS_ARM7_LOAD_MODULE,
	FEOS_ARM7_UNLOAD_MODULE,
	FEOS_ARM7_RETURN_MODULE,
	FEOS_ARM7_HEADPHONE_SLEEP,
	FEOS_ARM7_GET_SYSTEM_INFO,
};

#define FEOS_ARM9_WAKEUP_ADDR ((void*)0x02000000) // HACK

typedef struct
{
	void* data;
	word_t size;
	word_t bsssize;
	fxe_inmem_imports imps;
	int nrelocs;
	fxe2_reloc_t* relocs;
} FeOSLoadStruct;

typedef struct
{
	int type;
	union
	{
		FeOSLoadStruct* loadStruct;
		struct
		{
			instance_t hModule;
			int fifoCh;
		};
	};
} FeOSFifoMsg;

void installFeOSFIFO();
