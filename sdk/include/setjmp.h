//
// FeOS C Standard Library
// setjmp.h
//     Jump buffer functions
//

#pragma once
#include <stddef.h>

// r4..r11, sp & lr
typedef word_t jmp_buf[10];

// Remember that jmp_buf is still a pointer
EXTERNC int setjmp(jmp_buf);
EXTERNC void longjmp(jmp_buf, int);
