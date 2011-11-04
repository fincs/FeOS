#include <feos.h>

OamState *_oamMain, *_oamSub;
SpriteEntry *oamMain_mem, *oamSub_mem;

FEOSINIT void _oamInit()
{
	_oamMain = FeOS_GetMainOAM();
	_oamSub = FeOS_GetSubOAM();
	oamMain_mem = FeOS_GetOAMMemory(_oamMain);
	oamSub_mem = FeOS_GetOAMMemory(_oamSub);
}
