//
// FeOS C Standard Library
// stddef.h
//     Standard C definitions
//

#pragma once
#include <feosbase.h>

/** @file stddef.h
 *  \brief Standard C definitions
 */

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

#define offsetof(type, member) __builtin_offsetof(type, member) //!< Gets the offset of a member

typedef long_t ptrdiff_t; //!< Pointer difference type
typedef word_t size_t; //!< Size type
typedef long_t ssize_t; //!< Signed size type

#ifndef __cplusplus
#define NULL ((void*)0) //!< Null pointer
#else
#define NULL 0 //!< Null pointer
typedef decltype(nullptr) nullptr_t; //!< Type of C++11 `nullptr`
#endif

/** @} */
