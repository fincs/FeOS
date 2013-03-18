//
// FeOS C Standard Library
// limits.h
//     Information about integral types
//

#pragma once

/** @file limits.h
 *  \brief Information about integral types
 */

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

#define CHAR_BIT   8 //!< Number of bits in a `char`
#define SCHAR_MIN  (-0x80) //!< `signed char` minimum value
#define SCHAR_MAX  0x7F //!< `signed char` maximum value
#define UCHAR_MAX  0xFF //!< `unsigned char` maximum value
#define CHAR_MIN   SCHAR_MIN //!< `char` minimum value
#define CHAR_MAX   SCHAR_MAX //!< `char` maximum value
#define MB_LEN_MAX 4 //!< TODO: description
#define SHRT_MIN   (-0x8000) //!< `short` minimum value
#define SHRT_MAX   0x7FFF //!< `short` maximum value
#define USHRT_MAX  0xFFFF //!< `unsigned short` maximum value
#define LONG_MIN   (-0x80000000) //!< `long` minimum value
#define LONG_MAX   0x7FFFFFFF //!< `long` maximum value
#define ULONG_MAX  0xFFFFFFFF //!< `unsigned long` maximum value
#define INT_MIN    LONG_MIN //!< `int` minimum value
#define INT_MAX    LONG_MAX //!< `int` maximum value
#define UINT_MAX   ULONG_MAX //!< `unsigned int` maximum value
#define SIZE_MAX   UINT_MAX //!< `size_t` maximum value

/** @} */
