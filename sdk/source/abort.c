#include <feos.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#include <backtrace.h>
static int stackdumpCallback(void* address, int depth /* 0-based */, void* user_data);
#endif

void abort()
{
	fprintf(stderr, "abort() called!\n");
#ifdef DEBUG
	// Dynamically load FeOS_Backtrace in order to avoid creating
	// a dependence on feoscxx
	instance_t hCxxLib = FeOS_GetModule("feoscxx");
	if (hCxxLib)
	{
		typedef void (* backtraceFuncType)(BacktraceCallback callback, void* user_data);
		backtraceFuncType backtraceFunc = (backtraceFuncType) FeOS_FindSymbol(hCxxLib, "FeOS_Backtrace");
		if (backtraceFunc)
		{
			fprintf(stderr, "from:\n");
			backtraceFunc(stackdumpCallback, NULL);
		}
	}
#endif
	exit(-1);
}

#ifdef DEBUG
static int stackdumpCallback(void* address, int depth /* 0-based */, void* user_data)
{
	if (depth == 0) return 1; // skip call to abort()
	instance_t hInst = FeOS_ModuleFromAddress(address);
	if (!hInst || !~(word_t)hInst)
		fprintf(stderr, " -> 0x%08X\n", (word_t) address);
	else
		fprintf(stderr, " -> %s+0x%X\n", FeOS_GetModuleName(hInst), address - hInst);
	return 1;
}
#endif
