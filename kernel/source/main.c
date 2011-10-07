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

#include "hudicons.h"

void __SWIHandler();
void __ResetHandler();

PrintConsole* con; // Am I the only one that finds this offensive?
int conbg;

void chk_exit();

extern bool stdioRead;

volatile touchPosition touchPos;

void irq_vblank()
{
	// Done here because it's kernel mode code
	chk_exit();

	scanKeys();
	touchRead((touchPosition*)&touchPos);
	bgUpdate();
	oamSub.oamMemory[0].isHidden = !__inFAT;
	oamSub.oamMemory[1].isHidden = !stdioRead;
	oamUpdate(&oamSub);

	if (!stdioRead) keyboardUpdate();
}

u16* hudicon_gfx[2];

void videoInit()
{
	// Set up the main engine
	videoSetMode(MODE_3_2D);
	// Set up the sub engine
	videoSetModeSub(MODE_0_2D);

	// Set the VRAM layout
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_LCD);
	dmaFillWords(0, VRAM_C, 128*1024); // keyboardInit() does not clear the full tilemap...
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	// Initialize the console background
	con   = consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 0, 1, true, true);
	conbg = con->bgId;

	// Prepare sub screen OAM
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	dmaCopyWords(3, hudiconsTiles, SPRITE_GFX_SUB, hudiconsTilesLen);
	hudicon_gfx[0] = SPRITE_GFX_SUB;
	hudicon_gfx[1] = SPRITE_GFX_SUB + (32*32)/2;

	oamSet(&oamSub, 0, 8, 8, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
		hudicon_gfx[0], -1, false, false, false, false, false);
	oamSet(&oamSub, 1, 216, 8, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
		hudicon_gfx[1], -1, false, false, false, false, false);
	oamSub.oamMemory[0].isHidden = true;
	oamSub.oamMemory[1].isHidden = true;

	// Copy the HUD icon palette
	dmaCopyHalfWords(3, hudiconsPal, SPRITE_PALETTE_SUB, hudiconsPalLen);

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
	irqstack[0] &= ~0xF; // Force user mode :)
	irqstack[9] = (word_t) ForcefulExit;

	// Reset/clear the SWI stack
	extern word_t __sp_svc;
	__setSWIStack(&__sp_svc);
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

void FeOS_InitStreams();

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
	FeOS_InitStreams();

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
