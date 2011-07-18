//
// FeOS Standard Library
// feos.h
//     Main header file
//

#pragma once

#if defined(FEOS_USERMANAGER) && !defined(FEOS_NO_CRT)
#define FEOS_NO_CRT
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <feosbase.h>
#include <feosuser.h>
#include <feoscrt.h>

#ifdef __cplusplus
}
#endif
