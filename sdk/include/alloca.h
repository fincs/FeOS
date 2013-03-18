//
// FeOS C Standard Library
// alloca.h
//     Stack allocation function
//

#pragma once

/** @file alloca.h
 *  \brief Stack allocation function
 */

/** @addtogroup crt_mem String/memory management
 *  @{
 */

#define alloca(size) __builtin_alloca(size) //!< Allocates the specified amount of bytes in the stack.
#define _alloca alloca //!< Alias for alloca().

/** @} */
