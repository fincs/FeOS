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

// r4..r11, sp & lr
typedef word_t jmp_buf[10];

// Remember that jmp_buf is still a pointer
int setjmp(jmp_buf);
void longjmp(jmp_buf, int);

#ifdef __cplusplus
extern "C"
}
#endif
