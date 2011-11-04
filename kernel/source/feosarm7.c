#include "feos.h"
#include "fxe.h"

bool FeOS_swi_FifoSendAddress(int, void*);
bool FeOS_swi_FifoSendValue32(int, word_t);
bool FeOS_swi_FifoSendDatamsg(int, word_t, void*);
bool FeOS_swi_FifoCheckAddress(int);
bool FeOS_swi_FifoCheckValue32(int);
bool FeOS_swi_FifoCheckDatamsg(int);
void* FeOS_swi_FifoGetAddress(int);
word_t FeOS_swi_FifoGetValue32(int);
int FeOS_swi_FifoGetDatamsg(int, word_t, void*);

#define ADD_FUNC2(fn) ADD_FUNC_ALIAS(FeOS_swi_##fn, FeOS_##fn)

BEGIN_TABLE(FEOSARM7)
	ADD_FUNC2(FifoSendAddress),
	ADD_FUNC2(FifoSendValue32),
	ADD_FUNC2(FifoSendDatamsg),
	ADD_FUNC2(FifoCheckAddress),
	ADD_FUNC2(FifoCheckValue32),
	ADD_FUNC2(FifoCheckDatamsg),
	ADD_FUNC2(FifoGetAddress),
	ADD_FUNC2(FifoGetValue32),
	ADD_FUNC2(FifoGetDatamsg),
	ADD_FUNC(FeOS_FifoSetDatamsgHandler),
	ADD_FUNC(FeOS_FifoSetValue32Handler),
	ADD_FUNC(FeOS_FifoSetAddressHandler)
END_TABLE(FEOSARM7)

MAKE_FAKEMODULE(FEOSARM7)