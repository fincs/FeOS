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

extern void* _inst_FEOSDSAPI;

fxe_runtime_header _header_FEOSDSAPI =
{
	&_inst_FEOSDSAPI, // hThis
	"FEOSDSAPI", // name
	1, // refcount
	-1, // file
	NULL, // entrypoint
	MAKE_EXPORTSTRUCT(FEOSDSAPI), // exp
	{ 0, NULL }, // imp
	{ NULL, 0 }, // exidx
	NULL, // next
	NULL // prev
};

void* _inst_FEOSDSAPI = &_header_FEOSDSAPI;

