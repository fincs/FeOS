#include <feos.h>

void* __attribute__((weak)) __gnu_Unwind_Find_exidx(void* return_address, int* nrec)
{
	module_t hMod = LdrResolveAddr(return_address);
	if (!hMod) return 0;

	return LdrGetExidxTbl(hMod, nrec);
}
