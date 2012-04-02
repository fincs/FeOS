#include <feos.h>
#include "unwind.h"

/*
  typedef _Unwind_Reason_Code (*_Unwind_Trace_Fn) (_Unwind_Context *, void *);
  _Unwind_Reason_Code _Unwind_Backtrace(_Unwind_Trace_Fn,
					void*);
*/

typedef struct
{
	word_t demand_save_flags;
	word_t regs[16];
} phase2_vrs;

extern "C" _Unwind_Reason_Code __gnu_Unwind_Backtrace(_Unwind_Trace_Fn trace, void* trace_argument, phase2_vrs* entry_vrs);

typedef int (*bt_func)(void* address, int depth /* 0-based */, void* user_data);

typedef struct
{
	bt_func fn;
	void* user_data;
	int depth;
} bt_context;

static _Unwind_Reason_Code backtraceCb(_Unwind_Context*, void*);

extern "C" FEOS_EXPORT void FeOS_Backtrace(bt_func pFunc, void* user_data)
{
	bt_context ctx = { pFunc, user_data, -2 };
	_Unwind_Backtrace(backtraceCb, &ctx);
}

static phase2_vrs backtraceCtx;

// WARNING: this function may be called by the kernel in a privileged mode.
extern "C" FEOS_EXPORT void FeOS_BacktraceDump(word_t* pCtx, bt_func pFunc, void* user_data)
{
	bt_context ctx = { pFunc, user_data, -1 };
	backtraceCtx.demand_save_flags = 0xDEAD; // see hack in unwind.c
	__builtin_memcpy(backtraceCtx.regs, pCtx, 16*sizeof(word_t));
	__gnu_Unwind_Backtrace(backtraceCb, &ctx, &backtraceCtx);
}

static _Unwind_Reason_Code backtraceCb(_Unwind_Context* pUnwindCtx, void* _user)
{
	bt_context* pCtx = (bt_context*) _user;
	if (!~++pCtx->depth) // skip actual call to backtrace(), also unary operator abuse :D
		return _URC_OK;

	word_t* pRegs = (word_t*)pUnwindCtx + 1;
	return pCtx->fn((void*)pRegs[15] /* PC */, pCtx->depth, pCtx->user_data) ? _URC_OK : _URC_FAILURE;
}

int main(int argc, const char* argv[])
{
	if (argc != 2 || __builtin_strcmp(argv[1], "/r") != 0)
	{
		__builtin_printf("FeOS C++/debug support library\n");
		__builtin_printf("Usage: %s [/r]\n  /r - Stay resident\n", argv[0]);
		return 0;
	}
	FeOS_StayResident();
	return 0;
}
