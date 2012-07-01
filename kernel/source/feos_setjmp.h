#pragma once

// r4..r11, sp & lr
typedef word_t jmp_buf[10];

// Remember that jmp_buf is still a pointer
int setjmp(jmp_buf);
void longjmp(jmp_buf, int);
void longjmp0(jmp_buf, int); // allows '0' as return code
