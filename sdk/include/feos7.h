//
// FeOS Standard Library
// feos7.h
//     ARM7 utility functions
//

#pragma once

typedef void (*coopFifoDatamsgHandlerFunc)(int num_bytes, void* userdata);
typedef void (*coopFifoValue32HandlerFunc)(word_t value32, void* userdata);
typedef void (*coopFifoAddressHandlerFunc)(void* address, void* userdata);

void coopFifoSetDatamsgHandler(int channel, coopFifoDatamsgHandlerFunc handler, void* userdata);
void coopFifoSetValue32Handler(int channel, coopFifoValue32HandlerFunc handler, void* userdata);
void coopFifoSetAddressHandler(int channel, coopFifoAddressHandlerFunc handler, void* userdata);
fp_t coopIrqSet(word_t mask, fp_t fn);
word_t coopIrqCheck();
word_t coopIrqNext();
void coopTimerStart(int timer, int divider, hword_t ticks, fp_t callback);
