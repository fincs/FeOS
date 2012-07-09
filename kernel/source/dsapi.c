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
void videoReset();

static OamState* FeOS_GetMainOAM()
{
	return &oamMain;
}

static OamState* FeOS_GetSubOAM()
{
	return &oamSub;
}

static SpriteEntry* FeOS_GetOAMMemory(OamState* oam)
{
	sassert(oam == &oamMain || oam == &oamSub, "Invalid parameter");
	return oam->oamMemory;
}

// A custom version must be used because of the usage of swiWaitForVBlank and DC_FlushRange
static void _FeOS_oamInit(OamState* oam, SpriteMapping mapping, bool extPalette)
{
	int i;
	int extPaletteFlag = extPalette ? DISPLAY_SPR_EXT_PALETTE : 0;

	sassert(oam == &oamMain || oam == &oamSub, "Invalid parameter");

	oam->gfxOffsetStep = (mapping & 3) + 5;

	oam->spriteMapping = mapping;

	dmaFillWords(0, oam->oamMemory, 128*sizeof(SpriteEntry));

	for(i = 0; i < 128; i ++)
		oam->oamMemory[i].isHidden = true;

	for(i = 0; i < 32; i ++)
	{
		oam->oamRotationMemory[i].hdx = (1<<8);
		oam->oamRotationMemory[i].vdy = (1<<8);
	}

	FeOS_WaitForVBlank();

	FeOS_swi_DataCacheFlush(oam->oamMemory, 128*sizeof(SpriteEntry));

	if(oam == &oamMain)
	{
		dmaCopy(oam->oamMemory, OAM, 128*sizeof(SpriteEntry));

		REG_DISPCNT &= ~DISPLAY_SPRITE_ATTR_MASK;
		REG_DISPCNT |= DISPLAY_SPR_ACTIVE | (mapping & 0xffffff0) | extPaletteFlag;
	}else
	{
		dmaCopy(oam->oamMemory, OAM_SUB, 128*sizeof(SpriteEntry));

		REG_DISPCNT_SUB &= ~DISPLAY_SPRITE_ATTR_MASK;
		REG_DISPCNT_SUB |= DISPLAY_SPR_ACTIVE | (mapping & 0xffffff0) | extPaletteFlag;
	}

	oamAllocReset(oam);
}

#define TIMER_CR_32(n) (*(vu32*)(0x04000100+((n)<<2)))

void FeOS_swi_TimerWrite(int timer, word_t v);
void _TimerWrite(int timer, word_t v)
{
	TIMER_CR_32(timer&3) = v;
}

u16 FeOS_swi_TimerTick(int timer);
u16 _TimerTick(int timer)
{
	return TIMER_DATA(timer&3);
}

void FeOS_KeysUpdate()
{
	if (!bKeyUpd) scanKeys();
}

// A custom version must be used because of the usage of DC_FlushRange
void _FeOS_oamUpdate(OamState* oam)
{
	sassert(oam == &oamMain || oam == &oamSub, "Invalid parameter");

	if (bOAMUpd) return;

	FeOS_swi_DataCacheFlush(oam->oamMemory, 128*sizeof(SpriteEntry));

	if (oam == &oamMain)
		dmaCopy(oam->oamMemory, OAM, 128*sizeof(SpriteEntry));
	else
		dmaCopy(oam->oamMemory, OAM_SUB, 128*sizeof(SpriteEntry));
}

void _FeOS_bgUpdate()
{
	if (!bBgUpd) bgUpdate();
}

typedef struct
{
	void (* directMode)();
	void (* consoleMode)();
	int (* getMode)();
} modeshim_t;

static modeshim_t* pModeShim = NULL;

static void directModeShim()
{
	pModeShim ? pModeShim->directMode() : FeOS_swi_DirectMode();
}

static void consoleModeShim()
{
	pModeShim ? pModeShim->consoleMode() : FeOS_swi_ConsoleMode();
}

static int getModeShim()
{
	return pModeShim ? pModeShim->getMode() : GetCurMode();
}

static modeshim_t* installModeShim(modeshim_t* pNewShim)
{
	modeshim_t* pOldShim = pModeShim;
	if (pNewShim != (modeshim_t*)~0)
		pModeShim = pNewShim;
	return pOldShim;
}

static void FeOS_VideoReset()
{
	if (GetCurMode()) return;

	videoReset();
}

BEGIN_TABLE(FEOSDSAPI)
	ADD_FUNC_ALIAS(keysDown, FeOS_GetKeysDown),
	ADD_FUNC_ALIAS(keysHeld, FeOS_GetKeysHeld),
	ADD_FUNC_ALIAS(keysUp, FeOS_GetKeysUp),
	ADD_FUNC_ALIAS(keysDownRepeat, FeOS_GetKeysDownRepeat),
	ADD_FUNC_ALIAS(keysSetRepeat, FeOS_SetKeyRepeat),
	ADD_FUNC(FeOS_KeysUpdate),
	ADD_FUNC(FeOS_GetStylusPos),
	ADD_FUNC(FeOS_SetInterrupt),
	ADD_FUNC(FeOS_CheckPendingIRQs),
	ADD_FUNC(FeOS_WaitForIRQ),
	ADD_FUNC(FeOS_SetIRQWaitFunc),
	ADD_FUNC(FeOS_NextIRQ),
	ADD_FUNC_ALIAS(FeOS_swi_IrqDisable, FeOS_IrqDisable),
	ADD_FUNC_ALIAS(FeOS_swi_IrqEnable, FeOS_IrqEnable),
	ADD_FUNC_ALIAS(consoleModeShim, FeOS_ConsoleMode),
	ADD_FUNC_ALIAS(directModeShim, FeOS_DirectMode),
	ADD_FUNC_ALIAS(FeOS_swi_TimerWrite, FeOS_TimerWrite),
	ADD_FUNC_ALIAS(FeOS_swi_TimerTick, FeOS_TimerTick),
	ADD_FUNC_ALIAS(getModeShim, FeOS_GetMode),
	ADD_FUNC_ALIAS(installModeShim, FeOS_ModeShim),
	ADD_FUNC(FeOS_VideoReset),
	ADD_FUNC(dmaCopyWords),
	ADD_FUNC(dmaCopyHalfWords),
	ADD_FUNC(dmaFillWords),
	ADD_FUNC(dmaFillHalfWords),
	ADD_FUNC(memUncached),
	ADD_FUNC(memCached),
	ADD_FUNC(vramSetPrimaryBanks),
	ADD_FUNC(vramSetBanks_EFG),
	ADD_FUNC(vramRestorePrimaryBanks),
	ADD_FUNC(vramRestoreBanks_EFG),
	ADD_FUNC(vramSetBankA),
	ADD_FUNC(vramSetBankB),
	ADD_FUNC(vramSetBankC),
	ADD_FUNC(vramSetBankD),
	ADD_FUNC(vramSetBankE),
	ADD_FUNC(vramSetBankF),
	ADD_FUNC(vramSetBankG),
	ADD_FUNC(vramSetBankH),
	ADD_FUNC(vramSetBankI),
	ADD_FUNC(videoSetMode),
	ADD_FUNC(videoSetModeSub),
	ADD_FUNC(videoGetMode),
	ADD_FUNC(videoGetModeSub),
	ADD_FUNC(video3DEnabled),
	ADD_FUNC(setBrightness),
	ADD_FUNC(lcdSwap),
	ADD_FUNC(lcdMainOnTop),
	ADD_FUNC(lcdMainOnBottom),
	ADD_FUNC(powerOn),
	ADD_FUNC(powerOff)
END_TABLE(FEOSDSAPI)

MAKE_FAKEMODULE(FEOSDSAPI)

BEGIN_TABLE(FEOSDSSPR)
	ADD_FUNC(FeOS_GetMainOAM),
	ADD_FUNC(FeOS_GetSubOAM),
	ADD_FUNC(FeOS_GetOAMMemory),
	ADD_FUNC_ALIAS(_FeOS_oamInit, oamInit),
	ADD_FUNC_ALIAS(_FeOS_oamUpdate, oamUpdate),
	ADD_FUNC(oamDisable),
	ADD_FUNC(oamEnable),
	ADD_FUNC(oamGetGfxPtr),
	ADD_FUNC(oamAllocateGfx),
	ADD_FUNC(oamFreeGfx),
	ADD_FUNC(oamSetMosaic),
	ADD_FUNC(oamSetMosaicSub),
	ADD_FUNC(oamSet),
	ADD_FUNC(oamClear),
	ADD_FUNC(oamClearSprite),
	ADD_FUNC(oamRotateScale),
	ADD_FUNC(oamAffineTransformation),
	ADD_FUNC(oamGfxPtrToOffset)
END_TABLE(FEOSDSSPR)

MAKE_FAKEMODULE(FEOSDSSPR)

BEGIN_TABLE(FEOSDSBG)
	ADD_FUNC(bgSetRotate),
	ADD_FUNC(bgRotate),
	ADD_FUNC(bgSet),
	ADD_FUNC(bgSetRotateScale),
	ADD_FUNC(bgSetScale),
	ADD_FUNC(bgInit),
	ADD_FUNC(bgInitSub),
	ADD_FUNC_ALIAS(_FeOS_bgUpdate, bgUpdate),
	ADD_FUNC(bgSetControlBits),
	ADD_FUNC(bgClearControlBits),
	ADD_FUNC(bgSetPriority),
	ADD_FUNC(bgSetMapBase),
	ADD_FUNC(bgSetTileBase),
	ADD_FUNC(bgSetScrollf),
	ADD_FUNC(bgMosaicEnable),
	ADD_FUNC(bgMosaicDisable),
	ADD_FUNC(bgSetMosaic),
	ADD_FUNC(bgSetMosaicSub),
	ADD_FUNC(bgGetMapPtr),
	ADD_FUNC(bgGetGfxPtr),
	ADD_FUNC(bgGetPriority),
	ADD_FUNC(bgGetMapBase),
	ADD_FUNC(bgGetTileBase),
	ADD_FUNC(bgScrollf),
	ADD_FUNC(bgShow),
	ADD_FUNC(bgHide),
	ADD_FUNC(bgSetCenterf),
	ADD_FUNC(bgSetAffineMatrixScroll),
	ADD_FUNC(bgExtPaletteEnable),
	ADD_FUNC(bgExtPaletteEnableSub),
	ADD_FUNC(bgExtPaletteDisable),
	ADD_FUNC(bgExtPaletteDisableSub)
END_TABLE(FEOSDSBG)

MAKE_FAKEMODULE(FEOSDSBG)
