#include "feos.h"
#include "fxe.h"

typedef struct { int x, y; } styluspos_t;

extern volatile touchPosition touchPos;

void FeOS_GetStylusPos(styluspos_t* pos)
{
	pos->x = touchPos.px;
	pos->y = touchPos.py;
}

BEGIN_TABLE(FEOSDSAPI)
	ADD_FUNC_ALIAS(keysDown, FeOS_GetKeysDown),
	ADD_FUNC_ALIAS(keysHeld, FeOS_GetKeysHeld),
	ADD_FUNC_ALIAS(keysUp, FeOS_GetKeysUp),
	ADD_FUNC(FeOS_GetStylusPos)
END_TABLE(FEOSDSAPI)

MAKE_FAKEMODULE(FEOSDSAPI)
