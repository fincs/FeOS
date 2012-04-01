#include "feos.h"
#include "fxe.h"

typedef int (* prfunc)(int, void*, void*);

static instance_t cxxInst;
static prfunc prfuncs[3];

static int _failprfunc(int state, void* ucbp, void* context)
{
	return 9; // _URC_FAILURE
}

static prfunc _getPrfunc(int id)
{
	if (cxxInst) return prfuncs[id];
	cxxInst = FeOS_GetModule("feoscxx");
	if (!cxxInst) return _failprfunc;
	prfuncs[0] = FeOS_FindSymbol(cxxInst, "__aeabi_unwind_cpp_pr0");
	prfuncs[1] = FeOS_FindSymbol(cxxInst, "__aeabi_unwind_cpp_pr1");
	prfuncs[2] = FeOS_FindSymbol(cxxInst, "__aeabi_unwind_cpp_pr2");
	GetRuntimeData(cxxInst)->refcount ++; // prevent unloading
	return prfuncs[id];
}

int __aeabi_unwind_cpp_pr0(int state, void* ucbp, void* context)
{
	return _getPrfunc(0)(state, ucbp, context);
}

int __aeabi_unwind_cpp_pr1(int state, void* ucbp, void* context)
{
	return _getPrfunc(1)(state, ucbp, context);
}

int __aeabi_unwind_cpp_pr2(int state, void* ucbp, void* context)
{
	return _getPrfunc(2)(state, ucbp, context);
}
