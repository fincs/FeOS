#include "fxe.h"

#define MAX_EXIT_FUNCTIONS 256

static FeOSExitFunc exitstack[MAX_EXIT_FUNCTIONS];
static int exitstackptr = 0;

int FeOS_PushExitFunc(FeOSExitFunc func)
{
	if (exitstackptr == MAX_EXIT_FUNCTIONS) return 0;
	exitstack[exitstackptr++] = func;
	return 1;
}

void FeOS_CallExitFunc(int rc)
{
	if (exitstackptr == 0) exit(rc);
	exitstack[--exitstackptr](rc);
}

void FeOS_PopExitFunc()
{
	if (exitstackptr == 0) return;
	exitstackptr --;
}
