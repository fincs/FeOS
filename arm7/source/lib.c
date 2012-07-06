#include <stdlib.h>
#include <string.h>
#include "feos.h"

int __aeabi_idiv(int, int);
int __aeabi_idivmod(int, int);
unsigned int __aeabi_uidiv(unsigned int, unsigned int);
unsigned int __aeabi_uidivmod(unsigned int, unsigned int);

long long __aeabi_ldivmod(long long, long long);
unsigned long long __aeabi_uldivmod(unsigned long long, unsigned long long);

long long __aeabi_lmul(long long, long long);

#define ADD_FUNC(FUNC) {{(word_t)#FUNC}, {(word_t)FUNC}}
#define ADD_FUNC_ALIAS(FUNC, NAME) {{(word_t)#NAME}, {(word_t)FUNC}}

#define arm7_func_count (sizeof(arm7_funcs) / sizeof(fxe2_export_t))

void coopTimerStart(int timer, ClockDivider divider, u16 ticks, VoidFn callback)
{
	timer &= 3;
	TIMER_DATA(timer) = ticks;
	if (callback)
	{
		coopIrqSet(IRQ_TIMER(timer), callback);
		irqEnable(IRQ_TIMER(timer));
		TIMER_CR(timer) = TIMER_IRQ_REQ | divider | TIMER_ENABLE;
	}else
		TIMER_CR(timer) = divider | TIMER_ENABLE;
}

static const fxe2_export_t arm7_funcs[] =
{
	ADD_FUNC_ALIAS(coopWaitForVBlank, swiWaitForVBlank),
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
	ADD_FUNC(fifoCheckDatamsgLength),
	ADD_FUNC(fifoGetAddress),
	ADD_FUNC(fifoGetValue32),
	ADD_FUNC(fifoGetDatamsg),
	ADD_FUNC(swiDelay),
	ADD_FUNC_ALIAS(coop_swiIntrWaitCompat, swiIntrWait),
	ADD_FUNC(memcpy),
	ADD_FUNC(memcmp),
	ADD_FUNC(memset),
	ADD_FUNC(strcpy),
	ADD_FUNC(strcmp),
	ADD_FUNC(strncpy),
	ADD_FUNC(strncmp),
	ADD_FUNC(malloc),
	ADD_FUNC(free),
	ADD_FUNC(__aeabi_idiv),
	ADD_FUNC(__aeabi_idivmod),
	ADD_FUNC(__aeabi_uidiv),
	ADD_FUNC(__aeabi_uidivmod),
	ADD_FUNC(__aeabi_ldivmod),
	ADD_FUNC(__aeabi_uldivmod),
	ADD_FUNC(__aeabi_lmul),

	ADD_FUNC(coopFifoSetDatamsgHandler),
	ADD_FUNC(coopFifoSetValue32Handler),
	ADD_FUNC(coopFifoSetAddressHandler),
	ADD_FUNC(coopIrqSet),
	ADD_FUNC(coopIrqCheck),
	ADD_FUNC(coopIrqNext),
	ADD_FUNC(coopTimerStart)
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
	return false;
}
