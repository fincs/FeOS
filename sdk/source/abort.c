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
	module_t hCxxLib = LdrGetModule("feoscxx");
	if (hCxxLib)
	{
		typedef void (* backtraceFuncType)(BacktraceCallback callback, void* user_data);
		backtraceFuncType backtraceFunc = (backtraceFuncType) LdrFindSymbol(hCxxLib, "FeOS_Backtrace");
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
	module_t hMod = LdrResolveAddr(address);
	if (!hMod || !~(word_t)hMod)
		fprintf(stderr, " -> 0x%08X\n", (word_t) address);
	else
		fprintf(stderr, " -> %s+0x%X\n", LdrGetModuleName(hMod), address - hMod);
	return 1;
}
#endif
