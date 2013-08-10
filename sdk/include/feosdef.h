//
// FeOS Standard Library
// feosdef.h
//     Base definitions for FeOS
//

#pragma once
#include <stdbool.h>

/** @file feosdef.h
 *  \brief Base definitions for FeOS
 */

#define FEOS_EP_INIT 0x00000001
#define FEOS_EP_FINI 0x00000002
#define FEOS_EP_GETEXIDXTBL 0x00000003
#define FEOS_EP_MAIN 0x00000100

#define FEOS_RC_OK 0
#define FEOS_RC_ERR 0xFFFFFFFF

/** @addtogroup api_base Core FeOS API
 *  \brief Base FeOS API elements specific to operating system functionality.
 *  \code
 *  #include <feos.h>
 *  \endcode
 *  @{
 */

typedef unsigned char byte_t; //!< Unsigned 8-bit integer.
typedef unsigned short hword_t; //!< Unsigned 16-bit integer.
typedef unsigned int word_t; //!< Unsigned 32-bit integer.
typedef unsigned long long dword_t; //!< Unsigned 64-bit integer.
typedef signed char char_t; //!< Signed 8-bit integer.
typedef signed short short_t; //!< Signed 16-bit integer.
typedef signed int long_t; //!< Signed 32-bit integer.
typedef signed long long dlong_t; //!< Signed 64-bit integer.
typedef void* module_t; //!< Module handle.
typedef word_t bool_t;
typedef long_t off_t; //!< Offset datatype.
typedef void (* fp_t)(); //!< Function pointer datatype.

#ifndef ARM7
#define BIT(n) (1<<(n)) //!< Allows access to the nth bit.
#endif

//! \cond
extern char __modulebase[];

#ifdef __cplusplus
#define EXTERNC extern "C"
#define EXTERNC_DATA EXTERNC
#else
#define EXTERNC
#define EXTERNC_DATA extern
#endif
//! \endcond

#define FEOS_EXPORT __attribute__((__visibility__("default"))) //!< Exported symbol attribute.
#define FEOS_WEAK __attribute__((weak)) //!< Weak symbol attribute.
#define FEOS_DEPRECATED __attribute__((deprecated)) //!< Deprecated symbol attribute.

//! \cond
#define FEOSENTRY EXTERNC
//! \endcond
#define FEOSINIT __attribute__((constructor)) //!< Initialization function attribute. Makes a function be automatically called when a module is loaded.
#define FEOSFINI __attribute__((destructor)) //!< Deinitialization function attribute. Makes a function be automatically called when a module is unloaded.

/** @} */
