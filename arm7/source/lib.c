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

#define ADD_FUNC_(FUNC) {{(word_t)#FUNC}, {(word_t)FUNC}}
#define ADD_ALIAS(NAME, FUNC) {{(word_t)#NAME}, {(word_t)FUNC}}

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
	ADD_ALIAS(swiWaitForVBlank, coopWaitForVBlank),
	ADD_FUNC_(irqSet),
	ADD_FUNC_(irqClear),
	ADD_FUNC_(irqEnable),
	ADD_FUNC_(irqDisable),
	ADD_FUNC_(writePowerManagement),
	ADD_FUNC_(timerStart),
	ADD_FUNC_(timerPause),
	ADD_FUNC_(timerStop),
	ADD_FUNC_(timerElapsed),
	ADD_FUNC_(micSetAmp),
	ADD_FUNC_(micReadData8),
	ADD_FUNC_(micReadData12),
	ADD_FUNC_(fifoSendAddress),
	ADD_FUNC_(fifoSendValue32),
	ADD_FUNC_(fifoSendDatamsg),
	ADD_FUNC_(fifoSetAddressHandler),
	ADD_FUNC_(fifoSetValue32Handler),
	ADD_FUNC_(fifoSetDatamsgHandler),
	ADD_FUNC_(fifoCheckAddress),
	ADD_FUNC_(fifoCheckValue32),
	ADD_FUNC_(fifoCheckDatamsg),
	ADD_FUNC_(fifoCheckDatamsgLength),
	ADD_FUNC_(fifoGetAddress),
	ADD_FUNC_(fifoGetValue32),
	ADD_FUNC_(fifoGetDatamsg),
	ADD_FUNC_(swiDelay),
	ADD_ALIAS(swiIntrWait, coop_swiIntrWaitCompat),
	ADD_FUNC_(memcpy),
	ADD_FUNC_(memcmp),
	ADD_FUNC_(memset),
	ADD_FUNC_(strcpy),
	ADD_FUNC_(strcmp),
	ADD_FUNC_(strncpy),
	ADD_FUNC_(strncmp),
	ADD_FUNC_(malloc),
	ADD_FUNC_(free),
	ADD_FUNC_(__aeabi_idiv),
	ADD_FUNC_(__aeabi_idivmod),
	ADD_FUNC_(__aeabi_uidiv),
	ADD_FUNC_(__aeabi_uidivmod),
	ADD_FUNC_(__aeabi_ldivmod),
	ADD_FUNC_(__aeabi_uldivmod),
	ADD_FUNC_(__aeabi_lmul),

	ADD_FUNC_(coopFifoSetDatamsgHandler),
	ADD_FUNC_(coopFifoSetValue32Handler),
	ADD_FUNC_(coopFifoSetAddressHandler),
	ADD_FUNC_(coopIrqSet),
	ADD_FUNC_(coopIrqCheck),
	ADD_FUNC_(coopIrqNext),
	ADD_FUNC_(coopTimerStart),
	ADD_FUNC_(i2cWriteRegister),
	ADD_FUNC_(i2cReadRegister),
};

bool Ke7ResolveImp(word_t* addr, const char* name)
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
