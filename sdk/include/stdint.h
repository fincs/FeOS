//
// FeOS C Standard Library
// stdint.h
//     Integer type definitions
//

#pragma once

/** @file stdint.h
 *  \brief Integer type definitions
 */

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

typedef signed char int8_t; //!< Signed 8-bit integer
typedef unsigned char uint8_t; //!< Unsigned 8-bit integer
typedef signed short int16_t; //!< Signed 16-bit integer
typedef unsigned short uint16_t; //!< Unsigned 16-bit integer
typedef signed int int32_t; //!< Signed 32-bit integer
typedef unsigned int uint32_t; //!< Unsigned 32-bit integer
typedef signed long long int64_t; //!< Signed 64-bit integer
typedef unsigned long long uint64_t; //!< Unsigned 64-bit integer

typedef int32_t intptr_t; //!< Signed pointer-sized integer
typedef uint32_t uintptr_t; //!< Unsigned pointer-sized integer

/** @} */
