//
// FeOS C Standard Library
// limits.h
//     Information about integral types
//

#pragma once

#define CHAR_BIT   8
#define SCHAR_MIN  (-0x80)
#define SCHAR_MAX  0x7F
#define UCHAR_MAX  0xFF
#define CHAR_MIN   SCHAR_MIN
#define CHAR_MAX   SCHAR_MAX
#define MB_LEN_MAX 4
#define SHRT_MIN   (-0x8000)
#define SHRT_MAX   0x7FFF
#define USHRT_MAX  0xFFFF
#define LONG_MIN   (-0x80000000)
#define LONG_MAX   0x7FFFFFFF
#define ULONG_MAX  0xFFFFFFFF
#define INT_MIN    LONG_MIN
#define INT_MAX    LONG_MAX
#define UINT_MAX   ULONG_MAX
#define SIZE_MAX   UINT_MAX
