#include <feos.h>

OamState *_oamMain, *_oamSub;
SpriteEntry *oamMain_mem, *oamSub_mem;

FEOSINIT void _oamInit()
{
	_oamMain = DSGetMainOAM();
	_oamSub = DSGetSubOAM();
	oamMain_mem = DSGetOAMMemory(_oamMain);
	oamSub_mem = DSGetOAMMemory(_oamSub);
}
