//
// FeOS C Standard Library
// stdarg.h
//     Variadic function handling
//

#pragma once

/** @file stdarg.h
 *  \brief Variadic function handling
 */

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

typedef __builtin_va_list va_list; //!< Variadic argument list

#define va_start(v, n) __builtin_va_start(v, n) //!< Begins handling a va_list.
#define va_arg(v, t)   __builtin_va_arg(v, t) //!< Extracts an argument from a va_list.
#define va_copy(d, s)  __builtin_va_copy(d, s) //!< Copies a va_list.
#define va_end(v)      __builtin_va_end(v) //!< Finishes handling a va_list.

/** @} */
