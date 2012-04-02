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

typedef int (*BacktraceCallback)(void* address, int depth /* 0-based */, void* user_data);

void FeOS_Backtrace(BacktraceCallback callback, void* user_data);

#ifdef __cplusplus
}
#endif
