//
// FeOS Standard Library
// backtrace.h
//     Stack backtracing
//

#pragma once
#include <feos.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @file backtrace.h
 *  \brief Stack backtracing
 */

/** @addtogroup crt_cpp C++ facilities and runtime library
 *  @{
 */

typedef int (*BacktraceCallback)(void* address, int depth /* 0-based */, void* user_data); //!< Backtrace callback

void FeOS_Backtrace(BacktraceCallback callback, void* user_data); //!< Performs a backtrace.

/** @} */

#ifdef __cplusplus
}
#endif
