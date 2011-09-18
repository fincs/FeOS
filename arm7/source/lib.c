#include <stdlib.h>
#include <string.h>
#include "feos.h"

#define ADD_FUNC(FUNC) {{(word_t)#FUNC}, {(word_t)FUNC}}
#define ADD_FUNC_ALIAS(FUNC, NAME) {{(word_t)#NAME}, {(word_t)FUNC}}

#define arm7_func_count (sizeof(arm7_funcs) / sizeof(fxe2_export_t))

static const fxe2_export_t arm7_funcs[] =
{
	ADD_FUNC(swiWaitForVBlank),
	ADD_FUNC(irqSet),
	ADD_FUNC(irqClear),
	ADD_FUNC(irqEnable),
	ADD_FUNC(irqDisable),
	ADD_FUNC(writePowerManagement),
	ADD_FUNC(timerStart),
	ADD_FUNC(timerPause),
	ADD_FUNC(timerStop),
	ADD_FUNC(timerElapsed),
	ADD_FUNC(fifoSendAddress),
	ADD_FUNC(fifoSendValue32),
	ADD_FUNC(fifoSendDatamsg),
	ADD_FUNC(fifoSetAddressHandler),
	ADD_FUNC(fifoSetValue32Handler),
	ADD_FUNC(fifoSetDatamsgHandler),
	ADD_FUNC(fifoCheckAddress),
	ADD_FUNC(fifoCheckValue32),
	ADD_FUNC(fifoCheckDatamsg),
	//ADD_FUNC(fifoCheckDatamsgLength), // this function does not exist at all?!?!
	ADD_FUNC(fifoGetAddress),
	ADD_FUNC(fifoGetValue32),
	ADD_FUNC(fifoGetDatamsg),
	ADD_FUNC(swiDelay),
	ADD_FUNC(swiIntrWait),
	ADD_FUNC(memcpy),
	ADD_FUNC(memcmp),
	ADD_FUNC(memset),
	ADD_FUNC(strcpy),
	ADD_FUNC(strcmp),
	ADD_FUNC(strncpy),
	ADD_FUNC(strncmp)
};

bool FeOS_ResolveImp(word_t* addr, const char* name)
{
	register int i;
	for (i = 0; i < arm7_func_count; i ++)
		if (strcmp(name, arm7_funcs[i].name) == 0)
		{
			*addr = arm7_funcs[i].address;
			return true;
		}
	return true;
}
