//
// FeOS C Standard Library
// setjmp.h
//     Jump buffer functions
//

#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @file setjmp.h
 *  \brief Jump buffer functions
 */

/** @addtogroup crt_misc Misc utilities
 *  @{
 */

// r4..r11, sp & lr
typedef word_t jmp_buf[10]; //!< Jump buffer datatype

// Remember that jmp_buf is still a pointer
int setjmp(jmp_buf); //!< Prepares a jump buffer. \returns Zero after a setjmp() call, non-zero after a longjmp() call.
void longjmp(jmp_buf, int); //!< Jumps to a jump buffer with the specified return value.

/** @} */

#ifdef __cplusplus
extern "C"
}
#endif
