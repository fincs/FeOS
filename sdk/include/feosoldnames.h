//
// FeOS Standard Library
// feosoldnames.h
//     Old FeOS API names
//

#pragma once

typedef module_t instance_t;
typedef exidxinfo_t FeOS_GetExidxTbl_t;
typedef word_t keys_t;

#define FeOS_GetVersion KeGetOSVersion
#define FeOS_GetSystemInfo KeGetSysInfo

#define FeOS_GetInstance LdrGetSelf
#define FeOS_LoadModule LdrLoadModule
#define FeOS_FindSymbol LdrFindSymbol
#define FeOS_FreeModule LdrFreeModule
#define FeOS_LockModule LdrLockModule
#define FeOS_UnlockModule LdrUnlockModule

#define FeOS_ModuleGetExtraSize LdrResGetSize
#define FeOS_ModuleExtraRead LdrResRead
#define FeOS_ModuleExtraSeek LdrResSeek
#define FeOS_ModuleExtraTell LdrResTell

#define FeOS_GetModule LdrGetModule
#define FeOS_GetModuleName LdrGetModuleName
#define FeOS_EnumModules LdrEnumModules

#define FeOS_StayResident LdrBeginResidency
#define FeOS_EndResident LdrEndResidency

#define FeOS_Execute LdrExecuteArgv

#define FeOS_GetModuleExidxTbl LdrGetExidxTbl
#define FeOS_ModuleFromAddress LdrResolveAddr

#define FeOS_LoadARM7 DSLoadARM7
#define FeOS_FreeARM7 DSFreeARM7

#define FeOS_DrainWriteBuffer DC_DrainWriteBuffer
#define FeOS_WaitForMemAddr KeWaitForMemAddr

// WILL BE REMOVED
#define FeOS_SetAutoUpdate DSSetAutoUpdate
#define FeOS_GetAutoUpdate DSGetAutoUpdate
// END REMOVE

#define FeOS_SetSuspendMode DSSetSuspendMode
#define FeOS_GetSuspendMode DSGetSuspendMode

#define FeOS_GetDiskStats64 KeGetDiskStats
#define FeOS_GetMemStats KeGetMemStats

#define FeOS_GetTickCount KeGetTickCount

#define FeOS_div3232 KeDiv3232
#define FeOS_mod3232 KeMod3232
#define FeOS_div6432 KeDiv6432
#define FeOS_mod6432 KeMod6432
#define FeOS_div6464 KeDiv6464
#define FeOS_mod6464 KeMod6464
#define FeOS_sqrt32  KeSqrt32
#define FeOS_sqrt64  KeSqrt64

#define FeOS_CheckPendingIRQs DSProcessIRQs
#define FeOS_WaitForIRQ DSWaitForIRQ
#define FeOS_NextIRQ DSWaitForNextIRQ

#define FeOS_TimerWrite DSTimerWrite
#define FeOS_TimerTick DSTimerTick
#define FeOS_TimerStop DSTimerStop

#define FeOS_ConsoleMode DSConsoleMode
#define FeOS_DirectMode DSDirectMode
#define FeOS_GetMode DSGetMode

#define FeOS_VideoReset DSVideoReset

#define FeOS_SetStdin IoSetStdin
#define FeOS_SetStdout IoSetStdout
#define FeOS_SetStderr IoSetStderr
#define FeOS_OpenStream IoOpenStream
