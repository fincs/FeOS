//
// FeOS Standard Library
// feosuser.h
//     FeOS API
//

#pragma once
#include <feos.h>

#define FeOS_GetInstance() FeOS_hInstance

#define FEOS_EP_INIT 0x00000001
#define FEOS_EP_FINI 0x00000002
#define FEOS_EP_GETEXIDXTBL 0x00000003
#define FEOS_EP_MAIN 0x00000100

typedef struct
{
	void* table;
	int nentries;
} FeOS_GetExidxTbl_t;

#define FEOS_RC_OK 0
#define FEOS_RC_ERR 0xFFFFFFFF

instance_t FeOS_LoadModule(const char*);
void* FeOS_FindSymbol(instance_t, const char*);
void FeOS_FreeModule(instance_t);

int FeOS_Execute(int, const char*[]);

void FeOS_DebugPrint(const char*);
void FeOS_WaitForVBlank();

int FeOS_PushExitFunc(FeOSExitFunc func);
void FeOS_PopExitFunc();
