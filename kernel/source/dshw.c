#include "feos.h"
#include "loader.h"

typedef struct { int x, y; } styluspos_t;

extern volatile touchPosition touchPos;

void DSTouchRead(styluspos_t* pos)
{
	pos->x = touchPos.px;
	pos->y = touchPos.py;
}

void DSConsoleMode();
void DSDirectMode();
int DSGetCurMode();
void DSVideoReset();

static OamState* DSGetMainOAM()
{
	return &oamMain;
}

static OamState* DSGetSubOAM()
{
	return &oamSub;
}

static SpriteEntry* DSGetOAMMemory(OamState* oam)
{
	sassert(oam == &oamMain || oam == &oamSub, "Invalid parameter");
	return oam->oamMemory;
}

// A custom version must be used because of the usage of swiWaitForVBlank and DC_FlushRange
static void oamInit2(OamState* oam, SpriteMapping mapping, bool extPalette)
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

	DSWaitForVBlankRaw();

	KeDataCacheFlush(oam->oamMemory, 128*sizeof(SpriteEntry));

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

void DSTimerWrite(int timer, word_t v)
{
	TIMER_CR_32(timer&3) = v;
}

u16 DSTimerTick(int timer)
{
	return TIMER_DATA(timer&3);
}

void DSScanKeys()
{
	if (!bKeyUpd) scanKeys();
}

// A custom version must be used because of the usage of DC_FlushRange
void oamUpdate2(OamState* oam)
{
	sassert(oam == &oamMain || oam == &oamSub, "Invalid parameter");

	if (bOAMUpd) return;

	KeDataCacheFlush(oam->oamMemory, 128*sizeof(SpriteEntry));

	if (oam == &oamMain)
		dmaCopy(oam->oamMemory, OAM, 128*sizeof(SpriteEntry));
	else
		dmaCopy(oam->oamMemory, OAM_SUB, 128*sizeof(SpriteEntry));
}

void DSBgUpdate()
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
	pModeShim ? pModeShim->directMode() : DSDirectMode();
}

static void consoleModeShim()
{
	pModeShim ? pModeShim->consoleMode() : DSConsoleMode();
}

static int getModeShim()
{
	return pModeShim ? pModeShim->getMode() : DSGetCurMode();
}

static modeshim_t* DSModeShim(modeshim_t* pNewShim)
{
	modeshim_t* pOldShim = pModeShim;
	if (pNewShim != (modeshim_t*)~0)
		pModeShim = pNewShim;
	return pOldShim;
}

module_t DSLoadARM7(const char*, int*);
void DSFreeARM7(module_t, int);

BEGIN_TABLE(FEOSDSHW)
	ADD_ALIAS(DSConsoleMode, consoleModeShim),
	ADD_ALIAS(DSDirectMode, directModeShim),
	ADD_FUNC_(DSFreeARM7),
	ADD_FUNC_(DSGetMainOAM),
	ADD_ALIAS(DSGetMode, getModeShim),
	ADD_FUNC_(DSGetOAMMemory),
	ADD_FUNC_(DSGetSubOAM),
	ADD_FUNC_(DSLoadARM7),
	ADD_FUNC_(DSModeShim),
	ADD_FUNC_(DSProcessIRQs),
	ADD_FUNC_(DSSetSuspendMode),
	ADD_FUNC_(DSTimerTick),
	ADD_FUNC_(DSTimerWrite),
	ADD_FUNC_(DSVideoReset),
	ADD_FUNC_(DSWaitForIRQ),
	ADD_FUNC_(DSWaitForNextIRQ),
	ADD_FUNC_(bgClearControlBits),
	ADD_FUNC_(bgExtPaletteDisable),
	ADD_FUNC_(bgExtPaletteDisableSub),
	ADD_FUNC_(bgExtPaletteEnable),
	ADD_FUNC_(bgExtPaletteEnableSub),
	ADD_FUNC_(bgGetGfxPtr),
	ADD_FUNC_(bgGetMapBase),
	ADD_FUNC_(bgGetMapPtr),
	ADD_FUNC_(bgGetPriority),
	ADD_FUNC_(bgGetTileBase),
	ADD_FUNC_(bgHide),
	ADD_FUNC_(bgInit),
	ADD_FUNC_(bgInitSub),
	ADD_FUNC_(bgMosaicDisable),
	ADD_FUNC_(bgMosaicEnable),
	ADD_FUNC_(bgRotate),
	ADD_FUNC_(bgScrollf),
	ADD_FUNC_(bgSet),
	ADD_FUNC_(bgSetAffineMatrixScroll),
	ADD_FUNC_(bgSetCenterf),
	ADD_FUNC_(bgSetControlBits),
	ADD_FUNC_(bgSetMapBase),
	ADD_FUNC_(bgSetMosaic),
	ADD_FUNC_(bgSetMosaicSub),
	ADD_FUNC_(bgSetPriority),
	ADD_FUNC_(bgSetRotate),
	ADD_FUNC_(bgSetRotateScale),
	ADD_FUNC_(bgSetScale),
	ADD_FUNC_(bgSetScrollf),
	ADD_FUNC_(bgSetTileBase),
	ADD_FUNC_(bgShow),
	ADD_ALIAS(bgUpdate, DSBgUpdate),
	ADD_FUNC_(dmaCopyHalfWords),
	ADD_FUNC_(dmaCopyWords),
	ADD_FUNC_(dmaFillHalfWords),
	ADD_FUNC_(dmaFillWords),
	ADD_FUNC_(fifoCheckAddress),
	ADD_FUNC_(fifoCheckDatamsg),
	ADD_FUNC_(fifoCheckDatamsgLength),
	ADD_FUNC_(fifoCheckValue32),
	ADD_FUNC_(fifoGetAddress),
	ADD_FUNC_(fifoGetDatamsg),
	ADD_ALIAS(fifoGetRetValue32, DSFifoGetRetValue32),
	ADD_FUNC_(fifoGetValue32),
	ADD_FUNC_(fifoSendAddress),
	ADD_FUNC_(fifoSendDatamsg),
	ADD_FUNC_(fifoSendValue32),
	ADD_ALIAS(fifoSetAddressHandler, DSFifoSetAddressHandler),
	ADD_ALIAS(fifoSetDatamsgHandler, DSFifoSetDatamsgHandler),
	ADD_ALIAS(fifoSetValue32Handler, DSFifoSetValue32Handler),
	ADD_FUNC_(irqDisable),
	ADD_FUNC_(irqEnable),
	ADD_ALIAS(irqSet, DSIrqSet),
	ADD_FUNC_(keysDown),
	ADD_FUNC_(keysDownRepeat),
	ADD_FUNC_(keysHeld),
	ADD_FUNC_(keysSetRepeat),
	ADD_FUNC_(keysUp),
	ADD_FUNC_(lcdMainOnBottom),
	ADD_FUNC_(lcdMainOnTop),
	ADD_FUNC_(lcdSwap),
	ADD_FUNC_(memCached),
	ADD_FUNC_(memUncached),
	ADD_FUNC_(oamAffineTransformation),
	ADD_FUNC_(oamAllocateGfx),
	ADD_FUNC_(oamClear),
	ADD_FUNC_(oamClearSprite),
	ADD_FUNC_(oamDisable),
	ADD_FUNC_(oamEnable),
	ADD_FUNC_(oamFreeGfx),
	ADD_FUNC_(oamGetGfxPtr),
	ADD_FUNC_(oamGfxPtrToOffset),
	ADD_ALIAS(oamInit, oamInit2),
	ADD_FUNC_(oamRotateScale),
	ADD_FUNC_(oamSet),
	ADD_FUNC_(oamSetMosaic),
	ADD_FUNC_(oamSetMosaicSub),
	ADD_ALIAS(oamUpdate, oamUpdate2),
	ADD_FUNC_(powerOff),
	ADD_FUNC_(powerOn),
	ADD_ALIAS(scanKeys, DSScanKeys),
	ADD_FUNC_(setBrightness),
	ADD_ALIAS(touchRead, DSTouchRead),
	ADD_FUNC_(video3DEnabled),
	ADD_FUNC_(videoGetMode),
	ADD_FUNC_(videoGetModeSub),
	ADD_FUNC_(videoSetMode),
	ADD_FUNC_(videoSetModeSub),
	ADD_FUNC_(vramRestoreBanks_EFG),
	ADD_FUNC_(vramRestorePrimaryBanks),
	ADD_FUNC_(vramSetBankA),
	ADD_FUNC_(vramSetBankB),
	ADD_FUNC_(vramSetBankC),
	ADD_FUNC_(vramSetBankD),
	ADD_FUNC_(vramSetBankE),
	ADD_FUNC_(vramSetBankF),
	ADD_FUNC_(vramSetBankG),
	ADD_FUNC_(vramSetBankH),
	ADD_FUNC_(vramSetBankI),
	ADD_FUNC_(vramSetBanks_EFG),
	ADD_FUNC_(vramSetPrimaryBanks),
END_TABLE(FEOSDSHW)

MAKE_FAKEMODULE(FEOSDSHW)
