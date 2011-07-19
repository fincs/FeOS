#pragma once
#include <nds.h>

#define ITCM_DATA __attribute__((section(".itcm")))

#define FeOS_AllocStack(a) __builtin_alloca(a)

typedef unsigned int word_t;
typedef unsigned short hword_t;
typedef unsigned char byte_t;
typedef word_t bool_t;
typedef void* instance_t;

void DoTheUserMode();
void PrepareUserMode();

static inline bool AddressCheckMainRAM(const void* addr)
{
	return (word_t)addr >= 0x02000000 && (word_t)addr <= 0x03000000;
}

int FeOS_IsValidName(const char* path);

void FeOS_WaitForVBlank();

void FeOS_swi_DataCacheFlush(const void* mem, size_t size);
void FeOS_swi_DataCacheFlush();
void FeOS_swi_InstrCacheInvalidate(const void* mem, size_t size);
void FeOS_swi_InstrCacheInvalidateAll();
