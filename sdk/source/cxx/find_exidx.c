#include <feos.h>

void* __attribute__((weak)) __gnu_Unwind_Find_exidx(void* return_address, int* nrec)
{
	instance_t hInst = FeOS_ModuleFromAddress(return_address);
	if (!hInst) return 0;

	return FeOS_GetModuleExidxTbl(hInst, nrec);
}
