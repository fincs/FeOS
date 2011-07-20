#include <feos.h>
#include <stdlib.h>
#include <setjmp.h>

extern jmp_buf __exit_buf;
extern volatile int __exit_done;

void exit(int status)
{
	__exit_done = 1;
	longjmp(__exit_buf, status);
}
