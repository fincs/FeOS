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

//! \brief Sends a main RAM address to the ARM7 using a specified FIFO channel.
bool fifoSendAddress(int, void*);
//! \brief Sends a 32-bit value to the ARM7 using a specified FIFO channel.
bool fifoSendValue32(int, word_t);
//! \brief Sends a variable-size data message to the ARM7 using a specified FIFO channel.
bool fifoSendDatamsg(int, word_t, void*);
//! \brief Checks whether a certain FIFO channel has a main RAM address in the queue.
bool fifoCheckAddress(int);
//! \brief Checks whether a certain FIFO channel has a 32-bit value in the queue.
bool fifoCheckValue32(int);
//! \brief Checks whether a certain FIFO channel has a data message in the queue.
bool fifoCheckDatamsg(int);
//! \brief Checks the size of the data message in a certain FIFO channel's queue. -1 is returned when there is no message.
int fifoCheckDatamsgLength(int);
//! \brief Retrieves the main RAM address in the specified FIFO channel's queue.
void* fifoGetAddress(int);
//! \brief Retrieves the 32-bit value in the specified FIFO channel's queue.
word_t fifoGetValue32(int);
//! \brief Retrieves the variable-size data message in the specified FIFO channel's queue.
int fifoGetDatamsg(int, word_t, void*);
//! \brief Sets a handler for automatically receiving data messages from a certain FIFO channel.
void fifoSetDatamsgHandler(int, FifoDatamsgHandlerFunc, void*);
//! \brief Sets a handler for automatically receiving 32-bit values from a certain FIFO channel.
void fifoSetValue32Handler(int, FifoValue32HandlerFunc, void*);
//! \brief Sets a handler for automatically receiving main RAM addresses from a certain FIFO channel.
void fifoSetAddressHandler(int, FifoAddressHandlerFunc, void*);
//! \brief Waits for a 32-bit value to be received through the FIFO channel and returns it in a reentrant-safe way.
word_t fifoGetRetValue32(int);

// Misc functions

//! \brief Drains the CPU's writte buffer.
void FeOS_DrainWriteBuffer();
//! \brief Waits for a certain memory addresses to hold a certain value.
void FeOS_WaitForMemAddr(volatile byte_t*, byte_t);

//! \brief Auto-update modes.
enum { AUTOUPD_OAM = 1, AUTOUPD_BG, AUTOUPD_KEYS };
//! \brief Turns on or off a certain auto-update mode.
//!
//! Auto-updates is a mechanism that automatically issues calls to scanKeys(), bgUpdate() and oamUpdate()
//! at each VBlank. They are always on in console mode, so this function only has effect in direct mode.
void FeOS_SetAutoUpdate(int which, bool enable);
//! \brief Retrieves whether a certain auto-update mode is on.
bool FeOS_GetAutoUpdate(int which);

//! \brief Suspend modes.
enum { SuspendMode_Get = -1, SuspendMode_Disable = 0, SuspendMode_Normal, SuspendMode_Headphones };
//! \brief Changes FeOS' behaviour when the lid is closed.
int FeOS_SetSuspendMode(int mode);
//! \brief Retrieves the current lid closing behaviour.
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

//! \deprecated \brief Returns disk space stats.
bool FeOS_GetDiskStats(usagestats_t*) FEOS_DEPRECATED;
//! \brief Returns disk space stats.
bool FeOS_GetDiskStats64(usagestats64_t*);
//! \brief Returns RAM stats.
void FeOS_GetMemStats(usagestats_t*);

//! \cond
#define memBarrier() asm volatile("":::"memory")
#define barrierAccess(x) do { memBarrier(); (x); memBarrier(); } while(0)
//! \endcond

//! \brief Returns the number of 59.8261 Hz (VBlank) cycles passed since FeOS was booted.
int FeOS_GetTickCount();

/** @} */
