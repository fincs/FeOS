//
// FeOS C Standard Library
// stdarg.h
//     Variadic function handling
//

#pragma once

typedef __builtin_va_list va_list;

#define va_start(v, n) __builtin_va_start(v, n)
#define va_arg(v, t)   __builtin_va_arg(v, t)
#define va_end(v)      __builtin_va_end(v)
