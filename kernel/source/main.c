//#define USE_LIBFILESYSTEM

#include "feos.h"
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
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

void chk_exit();

extern bool stdioRead;

touchPosition touchPos;

void irq_vblank()
{
	// Done here because it's kernel mode code
	chk_exit();

	scanKeys();
	touchRead(&touchPos);
	bgUpdate();

#ifdef VIDEOTEST
	bgScroll(tilebg, -1, 0);
	bgScroll(bmpbg, -1, -1);
#endif

	if (!stdioRead) keyboardUpdate();
}

void videoInit()
{
	// Set up the main engine
	videoSetMode(MODE_3_2D | DISPLAY_BG_EXT_PALETTE);
	// Set up the sub engine
	videoSetModeSub(MODE_0_2D);

	// Set the VRAM layout
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_LCD);
	dmaFillWords(0, VRAM_C, 128*1024); // keyboardInit() does not clear the full tilemap...
	vramSetBankC(VRAM_C_SUB_BG);

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
	dmaFillHalfWords(0, bgGetGfxPtr(bmpbg), 256*256);
	
#ifdef VIDEOTEST
	drawbox(bgGetMapPtr(tilebg), 29, 21);
	drawbox(bgGetMapPtr(tilebg), 27, 19);
	drawbox(bgGetMapPtr(tilebg), 25, 17);

#define MKCOL(a) ((u32)(a) | ((u32)(a) << 8))

	dmaFillHalfWords(MKCOL(10 * 16 - 1), bgGetGfxPtr(bmpbg), 4*256);
#endif

	irqSet(IRQ_VBLANK, irq_vblank);
}

void InstallThunks();

void ForcefulExit()
{
	FeOS_CallExitFunc(E_APPKILLED);
}

word_t* __getIRQStack();
word_t* __getSWIStack();
void __setSWIStack(word_t*);

void KillCurrentApp_IRQ();
void KillCurrentApp_SWI();

void KillCurrentApp_IRQ()
{
	// Retrieve a pointer to the IRQ stack
	word_t* irqstack = __getIRQStack();

	// The only instructions that push anything to the IRQ stack are (in order of execution):
	//   (BIOS)   push {r0, r1, r2, r3, ip, lr}
	//   (libnds) stmfd sp!, {r0-r1,r12,lr} @ {spsr, IME, REG_BASE, lr_irq}
	// The IRQ stack now looks like this:
	//   spsr IME REG_BASE lr_irq r0 r1 r2 r3 ip lr
	// So, irqstack[9] contains the return address and irqstack[0], the SPSR

#ifdef __thumb__
	irqstack[0] |= BIT(5); // THUMB flag
#endif
	irqstack[9] = (word_t) ForcefulExit;
}

void KillCurrentApp_SWI()
{
	// Retrieve a pointer to the SWI stack
	word_t* swistack = __getSWIStack();

	// The only instruction that pushes anything to the SWI stack is:
	//   (swihook.s) push {r12, lr} @ spsr and return address
	// So, we only need to pop 2 values
	__setSWIStack(swistack + 2);

	// Switch back to User mode
	DoTheUserMode();

	// Kill the application
	ForcefulExit();
}

void KillCurrentApp()
{
	if (stdioRead) KillCurrentApp_SWI();
	else           KillCurrentApp_IRQ();
}

void OnFold()
{
}

void kbd_key(int key)
{
	if (stdioRead && key > 0)
	{
		putchar(key);
		return;
	}

	switch(key)
	{
		case DVK_MENU:
			if (keysHeld() & KEY_DOWN) KillCurrentApp();
			break;
		case DVK_FOLD:
			OnFold();
			break;
	}
}

volatile static bool _hasExited = false;
volatile static int _rc = 0;

void chk_exit()
{
	if (_hasExited)
		exit(_rc);
}

int main()
{
	videoInit();
	consoleDebugInit(DebugDevice_CONSOLE);

	Keyboard* kbd = keyboardDemoInit();
	kbd->OnKeyPressed = kbd_key;
	kbd->scrollSpeed = 0;
	keyboardShow();
	
	defaultExceptionHandler();
	SystemVectors.reset = (u32) __ResetHandler;
	SystemVectors.swi = (u32) __SWIHandler;
	setVectorBase(0);
	FeOS_ModuleListInit();

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
	PrepareUserMode();
	DoTheUserMode();
	iprintf("User mode OK\n\n");

	iprintf("Loading command prompt...\n");

	const char* argv[] = { "cmd" };

	_rc = FeOS_Execute(1, argv);
	_hasExited = true;

	for(;;) FeOS_WaitForVBlank();
}

void FeOS_DebugPrint(const char* text)
{
	fputs(text, stdout);
}
