
//
// FeOS Standard Library
// multifeos.h
//     MultiFeOS compatibility defines
//

#pragma once
#include <feos.h>

#define FeOS_CancelThread ThrFree
#define FeOS_CreateThread ThrCreate
#define FeOS_Yield ThrYield
#define FeOS_GetCurrentThread ThrGetSelf
#define FeOS_ExitThread ThrExit
#define FeOS_IsThreadActive ThrIsActive
#define FeOS_FreeThread ThrFree
#define FeOS_SetThreadPrio ThrSetPriority
#define FeOS_GetThreadRC ThrGetExitCode
#define FeOS_ThreadJoin ThrJoin
#define FeOS_DetachThread ThrDetach
#define FeOS_RunInContext ThrRunInContext
#define FeOS_Idle ThrIdle

#define FeOS_CreateProcess PsCreateFromArgv
#define FeOS_RunAsync PsCreateFromCmdLine

#define FeOS_NextIRQYield DSWaitForNextIRQ

#define FeOS_AcquireLock ThrAcquireLock
#define FeOS_ReleaseLock ThrReleaseLock
