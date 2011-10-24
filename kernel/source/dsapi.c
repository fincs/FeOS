#include "feos.h"
#include "fxe.h"

typedef struct { int x, y; } styluspos_t;

extern volatile touchPosition touchPos;

void FeOS_GetStylusPos(styluspos_t* pos)
{
	pos->x = touchPos.px;
	pos->y = touchPos.py;
}

void FeOS_swi_ConsoleMode();
void FeOS_swi_DirectMode();
int GetCurMode();

BEGIN_TABLE(FEOSDSAPI)
	ADD_FUNC_ALIAS(keysDown, FeOS_GetKeysDown),
	ADD_FUNC_ALIAS(keysHeld, FeOS_GetKeysHeld),
	ADD_FUNC_ALIAS(keysUp, FeOS_GetKeysUp),
	ADD_FUNC(FeOS_GetStylusPos),
	ADD_FUNC(FeOS_SetInterrupt),
	ADD_FUNC(FeOS_CheckPendingIRQs),
	ADD_FUNC(FeOS_WaitForIRQ),
	ADD_FUNC_ALIAS(FeOS_swi_IrqDisable, FeOS_IrqDisable),
	ADD_FUNC_ALIAS(FeOS_swi_IrqEnable, FeOS_IrqEnable),
	ADD_FUNC_ALIAS(FeOS_swi_ConsoleMode, FeOS_ConsoleMode),
	ADD_FUNC_ALIAS(FeOS_swi_DirectMode, FeOS_DirectMode),
	ADD_FUNC_ALIAS(GetCurMode, FeOS_GetMode)
END_TABLE(FEOSDSAPI)

MAKE_FAKEMODULE(FEOSDSAPI)
