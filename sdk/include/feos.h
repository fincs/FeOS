//
// FeOS Standard Library
// feos.h
//     Main header file
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <feosver.h>
#include <feosbase.h>

#ifndef ARM7
#include <feoserror.h>
#include <feosuser.h>
#include <feosmath.h>
#include <feosdsapi.h>
#include <feosdsspr.h>
#include <feosdsbg.h>
#else
#include <feos7.h>
#endif

#ifdef __cplusplus
}
#endif
