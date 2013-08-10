#pragma once
#include <nds.h>
#include "../../kernel/source/feos.h"
#include "../../kernel/source/loader.h"
#include "../../kernel/source/feosfifo.h"

void Ke7VBlankISR();

void coopFifoSetDatamsgHandler(int channel, FifoDatamsgHandlerFunc handler, void* userdata);
void coopFifoSetValue32Handler(int channel, FifoValue32HandlerFunc handler, void* userdata);
void coopFifoSetAddressHandler(int channel, FifoAddressHandlerFunc handler, void* userdata);
VoidFn coopIrqSet(word_t mask, VoidFn fn);
word_t coopIrqCheck();
void coopIrqWait(word_t mask);
word_t coopIrqNext();

void coop_swiIntrWaitCompat(word_t how, word_t what);
void coopTimerStart(int timer, ClockDivider divider, u16 ticks, VoidFn callback);

static inline void coopWaitForVBlank()
{
	coopIrqWait(IRQ_VBLANK);
}
