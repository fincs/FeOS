//
// FeOS C Standard Library
// assert.h
//     Assertion function
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DEBUG
#define assert(cond)       ((void)0)
#define sassert(cond, msg) ((void)0)
#else
#define assert(cond)       ((cond) ? (void)0 : __sassert(#cond, __FILE__, __LINE__, NULL))
#define sassert(cond, msg) ((cond) ? (void)0 : __sassert(#cond, __FILE__, __LINE__, msg))

void __attribute__((noreturn)) __sassert(const char* cond, const char* file, int line, const char* msg);
#endif

#ifdef __cplusplus
}
#endif
