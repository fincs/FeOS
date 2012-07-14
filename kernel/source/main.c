#include "feos.h"
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include "fxe.h"
#include "feosfifo.h"
#include <sys/iosupport.h>

#ifdef USE_LIBFILESYSTEM
#include <filesystem.h>
#endif

#include "hudicons.h"
#include "caret.h"

void __SWIHandler();
void __ResetHandler();

PrintConsole* con; // Am I the only one that finds this offensive?
int conbg;

void chk_exit();

extern bool stdioRead;

volatile touchPosition touchPos;

bool conMode = true, bOAMUpd = true, bBgUpd = true, bKeyUpd = true;

extern int keyBufferOffset;
extern int keyBufferLength;

int caretBlink = 0;

extern volatile bool inHeadphoneSleep;
volatile int vblankCounter = 0; // upper bound: ~2.275 years

void irq_vblank()
{
	// Done here because it's kernel mode code
	chk_exit();

	vblankCounter ++;

	if (bKeyUpd) scanKeys();
	touchRead((touchPosition*)&touchPos);

	if (inHeadphoneSleep) return;

	if (bBgUpd) bgUpdate();
	if (conMode)
	{
		oamSub.oamMemory[0].isHidden = !__inFAT;
		oamSub.oamMemory[1].isHidden = !stdioRead;
		oamMain.oamMemory[0].x = con->cursorX * 8;
		oamMain.oamMemory[0].y = con->cursorY * 8;
		caretBlink ++;
		if (caretBlink >= 30) caretBlink = 0;
		oamMain.oamMemory[0].isHidden = (caretBlink > 15);
	}
	if (bOAMUpd)
	{
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	if (conMode && !stdioRead)
	{
		int oldOff = keyBufferOffset, oldLen = keyBufferLength;
		keyboardUpdate();
		keyBufferOffset = oldOff, keyBufferLength = oldLen;
	}
}

u16* caret_gfx;
u16* hudicon_gfx[2];

void kbd_key();

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

	// Prepare main screen OAM
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	dmaCopyWords(3, caretTiles, SPRITE_GFX, caretTilesLen);
	caret_gfx = SPRITE_GFX;

	oamSet(&oamSub, 0, 8, 8, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
		hudicon_gfx[0], -1, false, false, false, false, false);
	oamSet(&oamSub, 1, 216, 8, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
		hudicon_gfx[1], -1, false, false, false, false, false);
	oamSub.oamMemory[0].isHidden = true;
	oamSub.oamMemory[1].isHidden = true;

	oamSet(&oamMain, 0, 0, 0, 0, 0, SpriteSize_8x8, SpriteColorFormat_16Color,
		caret_gfx, -1, false, false, false, false, false);
	oamMain.oamMemory[0].isHidden = true;

	// Copy the HUD icon palette
	dmaCopyHalfWords(3, hudiconsPal, SPRITE_PALETTE_SUB, hudiconsPalLen);
	dmaCopyHalfWords(3, caretPal, SPRITE_PALETTE, caretPalLen);

	irqSet(IRQ_VBLANK, irq_vblank);

	// Initialize the keyboard
	Keyboard* kbd = keyboardDemoInit();
	kbd->OnKeyPressed = kbd_key;
	kbd->scrollSpeed = 0;
	keyboardShow();
}

void InstallThunks();
void InstallConThunks();
void InstallConDummy();

void UnblockIORegion();
void BlockIORegion();

void videoReset()
{
	dmaFillWords(0, (void*)0x04000000, 0x56);
	dmaFillWords(0, (void*)0x04001008, 0x56);
	videoSetModeSub(0);
	lcdMainOnTop();

	vramDefault();

	VRAM_E_CR = 0;
	VRAM_F_CR = 0;
	VRAM_G_CR = 0;
	VRAM_H_CR = 0;
	VRAM_I_CR = 0;

	irqEnable(IRQ_VBLANK);
}

void InitConMode()
{
	int cS = enterCriticalSection();
	videoReset();
	videoInit();
	InstallConThunks();
	BlockIORegion();
	conMode = true;
	bOAMUpd = true, bBgUpd = true, bKeyUpd = true;
	leaveCriticalSection(cS);
}

void InitFreeMode()
{
	int cS = enterCriticalSection();
	videoReset();
	InstallConDummy();
	UnblockIORegion();
	conMode = false;
	bOAMUpd = true, bBgUpd = true, bKeyUpd = true;
	leaveCriticalSection(cS);
}

int GetCurMode()
{
	return (int)conMode;
}

void FeOS_SetAutoUpdate(int which, bool enable)
{
	if (conMode) return; // can't tweak
	switch (which)
	{
		case AUTOUPD_OAM:  bOAMUpd = enable; break;
		case AUTOUPD_BG:   bBgUpd  = enable; break;
		case AUTOUPD_KEYS: bKeyUpd = enable; break;
		default:                             break;
	}
}

bool FeOS_GetAutoUpdate(int which)
{
	if (conMode) return true;
	switch (which)
	{
		case AUTOUPD_OAM:  return bOAMUpd;
		case AUTOUPD_BG:   return bBgUpd;
		case AUTOUPD_KEYS: return bKeyUpd;
		default:           return false;
	}
}

void ForcefulExit()
{
	FeOS_ModuleExit(E_APPKILLED);
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

void ExcptHandler_C();

void error_die()
{
	iprintf("\nPress START to exit...\n");
	for (;;)
	{
		swiWaitForVBlank();
		if (keysDown() & KEY_START)
			exit(1);
	}
}

#define ANSIESC_RED "\x1b[31;1m"
#define ANSIESC_GREEN "\x1b[32;1m"
#define ANSIESC_YELLOW "\x1b[33;1m"
#define ANSIESC_DEFAULT "\x1b[39;1m"

#define MSG_OK   ANSIESC_GREEN "  OK\n" ANSIESC_DEFAULT
#define MSG_FAIL ANSIESC_RED "FAIL\n\n" ANSIESC_DEFAULT

int main()
{
	videoInit();
	consoleDebugInit(DebugDevice_CONSOLE);
	
	setExceptionHandler(ExcptHandler_C);
	SystemVectors.reset = (u32) __ResetHandler;
	SystemVectors.swi = (u32) __SWIHandler;
	setVectorBase(0);
	FeOS_ModuleListInit();
	FeOS_InitStreams();
	installFeOSFIFO();

	iprintf(
		"\n  FeOS kernel v" FEOS_VERSION_TEXT "\n"
		"  Written in 2010-12 by fincs\n");

#ifdef DEBUG
	iprintf(ANSIESC_YELLOW "  **DEBUG BUILD**\n");
	iprintf("  Built: " __DATE__ ", " __TIME__ "\n" ANSIESC_DEFAULT);
#endif
	iprintf("\nInitializing filesystem... ");
#ifndef USE_LIBFILESYSTEM
	if(!fatInitDefault())
#else
	if(!nitroFSInit())
#endif
	{
		iprintf(MSG_FAIL);
		iprintf("Make sure you have DLDI patched\n");
		iprintf("the kernel binary. If the issue\n");
		iprintf("persists, try using HBMenu.\n\n");
		iprintf("  http://devkitpro.org/hbmenu\n");
		iprintf("  http://feos.mtheall.com/forum\n");
		error_die();
	}
	iprintf(MSG_OK);

	iprintf("Initializing user mode...  ");
	InstallThunks();
	PrepareUserMode();
	DoTheUserMode();
	iprintf(MSG_OK);

#ifdef DEBUG
	iprintf("Loading debug library...   ");
	instance_t hCxxLib = LoadModule("feoscxx");
	if (!hCxxLib)
	{
		iprintf(MSG_FAIL);
		iprintf("The following file is missing\n");
		iprintf("or it may have been corrupted:\n");
		iprintf("  /data/FeOS/lib/feoscxx.fx2\n");
		error_die();
	}
	iprintf(MSG_OK);
#endif

	iprintf("Startup...\n\n");

	const char* argv[] = { "cmd", ":startup", NULL };

	_rc = FeOS_Execute(2, argv);
	_hasExited = true;

	for(;;) FeOS_WaitForVBlank();
}

void FeOS_DebugPrint(const char* text)
{
	fputs(text, stdout);
}
