//#define USE_LIBFILESYSTEM

#include "feos.h"
#include <fat.h>
#include <stdio.h>
#include "fxe.h"
#include <sys/iosupport.h>

#ifdef USE_LIBFILESYSTEM
#include <filesystem.h>
#endif

#include "uigfx_bin.h"
#include "uipal_bin.h"

void __SWIHandler();
void __ResetHandler();

PrintConsole* con; // Am I the only one that finds this offensive?
int conbg, tilebg, bmpbg;

// BEWARE: Crappy test functions ahead!

#define MAPIDX(a, x, y) a[((y) * 32) + (x)]

static inline void drawbox(u16* map, int x, int y)
{
	MAPIDX(map, x+0, y+0) = 8;
	MAPIDX(map, x+1, y+0) = 9;
	MAPIDX(map, x+2, y+0) = 10;
	MAPIDX(map, x+0, y+1) = 11;
	MAPIDX(map, x+1, y+1) = 2;
	MAPIDX(map, x+2, y+1) = 11 | BIT(10);
	MAPIDX(map, x+0, y+2) = 8 | BIT(11);
	MAPIDX(map, x+1, y+2) = 9 | BIT(11);
	MAPIDX(map, x+2, y+2) = 10 | BIT(11);
}

void irq_vblank()
{
	//static int x = 0;
	scanKeys();
	bgUpdate();

	bgScroll(tilebg, -1, 0);
	bgScroll(bmpbg, -1, -1);
}

void videoInit()
{
	// Set up the main engine
	videoSetMode(MODE_3_2D | DISPLAY_BG_EXT_PALETTE);
	// Allocate some VRAM for our backgrounds
	vramSetBankA(VRAM_A_MAIN_BG);

	// Initialize three backgrounds: console, text and bitmap
	con    = consoleInit(NULL, 2, BgType_Text4bpp, BgSize_T_256x256, 0, 1, true, true);
	conbg  = con->bgId;
	tilebg = bgInit(1, BgType_Text8bpp, BgSize_T_256x256, 1, 2);
	bmpbg  = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 4, 0);

	// Set the priority: console->text->bitmap->{3D}
	bgSetPriority(conbg,  3);
	bgSetPriority(tilebg, 2);
	bgSetPriority(bmpbg,  1);
	bgWrapOn(bmpbg);

	// Clear the text and bitmap backgrounds
	dmaFillHalfWords(0, bgGetMapPtr(tilebg), 2*1024);
	//dmaFillHalfWords(0, bgGetGfxPtr(tilebg), 16*1024);
	dmaCopyHalfWords(3, uigfx_bin, bgGetGfxPtr(tilebg), 32*1024);
	vramSetBankE(VRAM_E_LCD);
	dmaCopyHalfWords(3, uipal_bin, VRAM_E_EXT_PALETTE[1], 2*256);
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	
	drawbox(bgGetMapPtr(tilebg), 29, 21);
	drawbox(bgGetMapPtr(tilebg), 27, 19);
	drawbox(bgGetMapPtr(tilebg), 25, 17);
	dmaFillHalfWords(0, bgGetGfxPtr(bmpbg), 256*256);

#define MKCOL(a) ((u32)(a) | ((u32)(a) << 8))

	dmaFillHalfWords(MKCOL(10 * 16 - 1), bgGetGfxPtr(bmpbg), 4*256);

	irqSet(IRQ_VBLANK, irq_vblank);
}

void InstallThunks();

int main()
{
	videoInit();
	swiWaitForVBlank();
	
	defaultExceptionHandler();
	SystemVectors.reset = (u32) __ResetHandler;
	SystemVectors.swi = (u32) __SWIHandler;
	setVectorBase(0);
	PrepareUserMode();

	iprintf(
		"FeOS kernel\n"
		"===========\n\n"
		"Copyright (c) 2010-11, fincs\n\n");

	iprintf("Initializing filesystem...\n");
#ifndef USE_LIBFILESYSTEM
	if(!fatInitDefault())
#else
	if(!nitroFSInit())
#endif
	{
		iprintf("Filesystem error\n");
		for(;;)
			swiWaitForVBlank();
	}
	iprintf("Filesystem OK\n");

	InstallThunks();
	DoTheUserMode();
	iprintf("User mode OK\n\n");

	iprintf("Loading test executable...\n");
	instance_t hTest = LoadModule("/data/FeOS/sys/testbin.fx2");
	if(!hTest)
	{
		iprintf("Failure!\n");
		for(;;)
			FeOS_WaitForVBlank();
	}

	FeOSMain entrypoint = GetRuntimeData(hTest)->entrypoint;

	const char* aCmdLine = "This works!";

	iprintf("Press Start to begin...\n\n");
	for(;;){ if(keysDown() & KEY_START) break; FeOS_WaitForVBlank(); }

	entrypoint(FEOS_EP_MAIN, 1, (word_t)&aCmdLine, 0);
	
	FreeModule(hTest);

	for(;;)
		FeOS_WaitForVBlank();
}

void FeOS_DebugPrint(const char* text)
{
	fputs(text, stdout);
}
