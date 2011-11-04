//
// FeOS Standard Library
// feosuser.h
//     FeOS API
//

#pragma once
#include <feos.h>

#define FeOS_GetInstance() FeOS_hInstance

typedef struct
{
	void* table;
	int nentries;
} FeOS_GetExidxTbl_t;

typedef void (*FifoAddressHandlerFunc)(void*, void*);
typedef void (*FifoValue32HandlerFunc)(word_t, void*);
typedef void (*FifoDatamsgHandlerFunc)(int, void*);

instance_t FeOS_LoadModule(const char*);
void* FeOS_FindSymbol(instance_t, const char*);
void FeOS_FreeModule(instance_t);
void FeOS_LockModule(instance_t);
void FeOS_UnlockModule(instance_t);

#define FeOS_StayResident() FeOS_LockModule(FeOS_hInstance)
#define FeOS_EndStayResident() FeOS_UnlockModule(FeOS_hInstance)

instance_t FeOS_LoadARM7(const char*, int*);
void FeOS_FreeARM7(instance_t, int);

int FeOS_Execute(int, const char*[]);

void FeOS_DebugPrint(const char*);
void FeOS_WaitForVBlank();

void FeOS_DataCacheFlush(const void*, word_t);
void FeOS_DataCacheFlushAll();
void FeOS_InstrCacheInvalidate(const void*, word_t);
void FeOS_InstrCacheInvalidateAll();

int FeOS_PushExitFunc(FeOSExitFunc func);
void FeOS_PopExitFunc();

void* FeOS_GetModuleExidxTbl(instance_t, int*);
void* FeOS_ModuleFromAddress(void*);

bool FeOS_FifoSendAddress(int, void*);
bool FeOS_FifoSendValue32(int, word_t);
bool FeOS_FifoSendDatamsg(int, word_t, void*);
bool FeOS_FifoCheckAddress(int);
bool FeOS_FifoCheckValue32(int);
bool FeOS_FifoCheckDatamsg(int);
void* FeOS_FifoGetAddress(int);
word_t FeOS_FifoGetValue32(int);
int FeOS_FifoGetDatamsg(int, word_t, void*);
void FeOS_FifoSetDatamsgHandler(int, FifoDatamsgHandlerFunc, void*);
void FeOS_FifoSetValue32Handler(int, FifoValue32HandlerFunc, void*);
void FeOS_FifoSetAddressHandler(int, FifoAddressHandlerFunc, void*);

// Misc functions
int FeOS_SuspendIRQ_t();
void FeOS_RestoreIRQ_t(int);
void FeOS_DrainWriteBuffer();
void FeOS_WaitForMemAddr(volatile byte_t*, byte_t);
