#include "feos.h"
#include "fxe.h"

instance_t FeOS_swi_LoadModule_ARM7(const char*, int*);
void FeOS_swi_FreeModule_ARM7(instance_t, int);

bool FeOS_swi_FifoSendAddress(int, void*);
bool FeOS_swi_FifoSendValue32(int, word_t);
bool FeOS_swi_FifoSendDatamsg(int, word_t, void*);
bool FeOS_swi_FifoCheckAddress(int);
bool FeOS_swi_FifoCheckValue32(int);
bool FeOS_swi_FifoCheckDatamsg(int);
void* FeOS_swi_FifoGetAddress(int);
word_t FeOS_swi_FifoGetValue32(int);
int FeOS_swi_FifoGetDatamsg(int, word_t, void*);

#define ADD_FUNC2(fn) ADD_FUNC_ALIAS(FeOS_swi_Fifo##fn, fifo##fn)

BEGIN_TABLE(FEOSARM7)
	ADD_FUNC_ALIAS(FeOS_swi_LoadModule_ARM7, FeOS_LoadARM7),
	ADD_FUNC_ALIAS(FeOS_swi_FreeModule_ARM7, FeOS_FreeARM7),
	ADD_FUNC2(SendAddress),
	ADD_FUNC2(SendValue32),
	ADD_FUNC2(SendDatamsg),
	ADD_FUNC2(CheckAddress),
	ADD_FUNC2(CheckValue32),
	ADD_FUNC2(CheckDatamsg),
	ADD_FUNC2(CheckDatamsgLength),
	ADD_FUNC2(GetAddress),
	ADD_FUNC2(GetValue32),
	ADD_FUNC2(GetDatamsg),
	ADD_FUNC_ALIAS(FeOS_FifoSetDatamsgHandler, fifoSetDatamsgHandler),
	ADD_FUNC_ALIAS(FeOS_FifoSetValue32Handler, fifoSetValue32Handler),
	ADD_FUNC_ALIAS(FeOS_FifoSetAddressHandler, fifoSetAddressHandler)
END_TABLE(FEOSARM7)

MAKE_FAKEMODULE(FEOSARM7)