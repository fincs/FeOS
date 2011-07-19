//
// FeOS C Standard Library
// stdio.h
//     Input/output functions
//

#pragma once
#include <stddef.h>
#include <limits.h>
#include <feosuser.h>

#ifdef __cplusplus
extern "C"
{
#endif

int printf(const char*, ...) __attribute__ ((format (printf, 1, 2)));

#ifdef __cplusplus
}
#endif
