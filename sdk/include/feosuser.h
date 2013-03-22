//
// FeOS Standard Library
// feosuser.h
//     FeOS API
//

#pragma once
#include <feos.h>

/** @file feosuser.h
 *  \brief Main FeOS Core API routines
 */

/** @addtogroup api_base Core FeOS API
 *  @{
 */

//! \brief Get the instance handle of the caller's module
#define FeOS_GetInstance() FeOS_hInstance

//! \brief Exception index table datatype
//! \headerfile feos.h <feos.h>
typedef struct
{
	void* table;
	int nentries;
} FeOS_GetExidxTbl_t;

//! \brief Execution status handle
typedef void* execstat_t;

typedef void (*FifoAddressHandlerFunc)(void*, void*); //!< FIFO address handler callback
typedef void (*FifoValue32HandlerFunc)(word_t, void*); //!< FIFO 32-bit value handler callback
typedef void (*FifoDatamsgHandlerFunc)(int, void*); //!< FIFO datamsg handler callback

//! \brief Loads a module of the specified name. Pathless extensionless module names are recommended.
instance_t FeOS_LoadModule(const char*);
//! \brief Finds a symbol in a module.
void* FeOS_FindSymbol(instance_t, const char*);
//! \brief Frees a module.
void FeOS_FreeModule(instance_t);
//! \brief Increases a module's reference count.
void FeOS_LockModule(instance_t);
//! \brief Decreases a module's reference count.
void FeOS_UnlockModule(instance_t);

//! \brief (Low-level) Gets the size of any extra data appended to the module file.
int FeOS_ModuleGetExtraSize(instance_t);
//! \brief (Low-level) Reads data appended to the module file.
int FeOS_ModuleExtraRead(instance_t, void*, word_t);
//! \brief (Low-level) Changes the reading pointer in the module file extra data.
int FeOS_ModuleExtraSeek(instance_t, int, int);
//! \brief (Low-level) Gets the position of the reading pointer of the module file extra data.
int FeOS_ModuleExtraTell(instance_t);

//! \brief Return value of ModuleEnumCallback
enum { ENUM_BREAK = 0, ENUM_CONTINUE = 1 };
//! \brief Module enumeration callback
typedef int (* ModuleEnumCallback)(instance_t, const char*, word_t, void*);

//! \brief Gets the handle of a module by name. \returns Null pointer if the module is not loaded
instance_t FeOS_GetModule(const char*);
//! \brief Gets the name of a module.
const char* FeOS_GetModuleName(instance_t);
//! \brief Enumerates all modules.
void FeOS_EnumModules(ModuleEnumCallback, void*);

//! \brief Increases the reference count of the callers' module.
#define FeOS_StayResident() FeOS_LockModule(FeOS_hInstance)
//! \brief Decreases the reference count of the callers' module.
#define FeOS_EndStayResident() FeOS_UnlockModule(FeOS_hInstance)

execstat_t FeOS_ExecStatusCreate(); //!< (Low-level) Creates a new execution status object.
void FeOS_ExecStatusAddRef(execstat_t); //!< (Low-level) Increases the reference count of an execstat object.
void FeOS_ExecStatusRelease(execstat_t); //!< (Low-level) Decreases the reference count of an execstat object.
void FeOS_SetCurExecStatus(execstat_t); //!< (Low-level) Sets the current execution status.
execstat_t FeOS_GetCurExecStatus(); //!< (Low-level) Gets the current execution status.

//! \brief Executes the specified argc and argv.
int FeOS_Execute(int, const char*[]);

//! \brief Flushes a range of memory in the data cache.
void DC_FlushRange(const void*, word_t);
//! \brief Flushes the data cache.
void DC_FlushAll();
//! \brief Invalidates a range of memory in the instruction cache.
void IC_InvalidateRange(const void*, word_t);
//! \brief Invalidates the instruction cache.
void IC_InvalidateAll();

//! \brief Extracts the exception index table of a module. The number of entries is passed back through the `int*` parameter.
void* FeOS_GetModuleExidxTbl(instance_t, int*);
//! \brief Gets the module that a certain address belongs to.
instance_t FeOS_ModuleFromAddress(void*);

//! \brief Loads an ARM7-side module. Full path should be supplied. A FIFO channel number is returned through the `int*` parameter.
instance_t FeOS_LoadARM7(const char*, int*);
//! \brief Unloads an ARM7-side module. The FIFO channel number must also be passed.
void FeOS_FreeARM7(instance_t, int);

bool fifoSendAddress(int, void*);
bool fifoSendValue32(int, word_t);
bool fifoSendDatamsg(int, word_t, void*);
bool fifoCheckAddress(int);
bool fifoCheckValue32(int);
bool fifoCheckDatamsg(int);
int fifoCheckDatamsgLength(int);
void* fifoGetAddress(int);
word_t fifoGetValue32(int);
int fifoGetDatamsg(int, word_t, void*);
void fifoSetDatamsgHandler(int, FifoDatamsgHandlerFunc, void*);
void fifoSetValue32Handler(int, FifoValue32HandlerFunc, void*);
void fifoSetAddressHandler(int, FifoAddressHandlerFunc, void*);

// Misc functions
void FeOS_DrainWriteBuffer();
void FeOS_WaitForMemAddr(volatile byte_t*, byte_t);

enum { AUTOUPD_OAM = 1, AUTOUPD_BG, AUTOUPD_KEYS };
void FeOS_SetAutoUpdate(int which, bool enable);
bool FeOS_GetAutoUpdate(int which);

enum { SuspendMode_Get = -1, SuspendMode_Disable = 0, SuspendMode_Normal, SuspendMode_Headphones };
int FeOS_SetSuspendMode(int mode);

#define FeOS_GetSuspendMode() FeOS_SetSuspendMode(SuspendMode_Get)

//! \headerfile feos.h <feos.h>
typedef struct
{
	word_t total, free, used;
} usagestats_t;

//! \headerfile feos.h <feos.h>
typedef struct
{
	dword_t total, free, used;
} usagestats64_t;

bool FeOS_GetDiskStats(usagestats_t*) FEOS_DEPRECATED;
bool FeOS_GetDiskStats64(usagestats64_t*);
void FeOS_GetMemStats(usagestats_t*);

#define memBarrier() asm volatile("":::"memory")
#define barrierAccess(x) do { memBarrier(); (x); memBarrier(); } while(0)

int FeOS_GetTickCount();

/** @} */
