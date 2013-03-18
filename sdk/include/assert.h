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

/** @file assert.h
 *  \brief Assertion function
 */

#if !defined(DEBUG) && !defined(DOXYGEN)
#define assert(cond)       ((void)0)
#define sassert(cond, msg) ((void)0)
#else

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

//! \brief Checks if the specified condition is true. Calls abort() if false.
#define assert(cond)       ((cond) ? (void)0 : __sassert(#cond, __FILE__, __LINE__, NULL))
//! \brief Same as assert(), but allows specifying a custom error message.
#define sassert(cond, msg) ((cond) ? (void)0 : __sassert(#cond, __FILE__, __LINE__, msg))

/** @} */

void __attribute__((noreturn)) __sassert(const char* cond, const char* file, int line, const char* msg);
#endif

#ifdef __cplusplus
}
#endif
