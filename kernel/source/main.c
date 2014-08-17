#include "feos.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "loader.h"
#include "feosfifo.h"
#include <sys/iosupport.h>

#include "hudicons.h"
#include "caret.h"

// Replacement for newlib iprintf
ssize_t iprintf(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	ssize_t ret = vfiprintf(stdout, fmt, va);
	va_end(va);
	return ret;
}

void __SWIHandler();
void __ResetHandler();

PrintConsole* con;
int conbg;

static void _CheckExit();
static void _KeybdKey(int);

extern bool stdioRead;

volatile touchPosition touchPos;

static int mainRequestHW(threadEP_t func, void* userData);
static hwreqfunc_t hwReqFunc = mainRequestHW;
static volatile bool hwOwned = true;

extern int keyBufferOffset;
extern int keyBufferLength;

int caretBlink = 0;

extern volatile bool inHeadphoneSleep;
volatile int vblankCounter = 0; // upper bound: ~2.275 years

static void KeVBlankISR()
{
	// Done here because it's kernel mode code
	_CheckExit();

	vblankCounter ++;

	touchRead((touchPosition*)&touchPos);

	if (!hwOwned) return;

	scanKeys();
	bgUpdate();

	if (inHeadphoneSleep) return;

	oamSub.oamMemory[0].isHidden = !__inFAT;
	oamSub.oamMemory[1].isHidden = !stdioRead;
	oamMain.oamMemory[0].x = con->cursorX * 8;
	oamMain.oamMemory[0].y = con->cursorY * 8;
	caretBlink ++;
	if (caretBlink >= 30) caretBlink = 0;
	oamMain.oamMemory[0].isHidden = (caretBlink > 15);

	oamUpdate(&oamMain);
	oamUpdate(&oamSub);

	if (!stdioRead)
	{
		int oldOff = keyBufferOffset, oldLen = keyBufferLength;
		keyboardUpdate();
		keyBufferOffset = oldOff, keyBufferLength = oldLen;
	}
}

u16* caret_gfx;
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

	if (hwOwned)
		irqSet(IRQ_VBLANK, KeVBlankISR);

	// Initialize the keyboard
	Keyboard* kbd = keyboardDemoInit();
	kbd->OnKeyPressed = _KeybdKey;
	kbd->scrollSpeed = 0;
	keyboardShow();
}

void IoInstallThunks();
void IoRestoreStdStreams();
void IoMothballStdStreams();

void DSVideoReset()
{
	// Not my fault if this func is inappropiately used!
	// (This func used to block attempts to call it when the HW was owned by main.c)

	dmaFillWords(0, (void*)0x04000000, 0x56);
	dmaFillWords(0, (void*)0x04001008, 0x56);
	videoSetModeSub(0);
	lcdMainOnTop();

	vramDefault();
	setBrightness(3, 0);

	VRAM_E_CR = 0;
	VRAM_F_CR = 0;
	VRAM_G_CR = 0;
	VRAM_H_CR = 0;
	VRAM_I_CR = 0;

	irqEnable(IRQ_VBLANK);
}

int mainRequestHW(threadEP_t func, void* userData)
{
	int cS = enterCriticalSection();
	hwOwned = false;
	DSVideoReset();
	IoMothballStdStreams();
	leaveCriticalSection(cS);

	int rc = func(userData);

	DSVideoReset();
	videoInit();
	IoRestoreStdStreams();
	hwOwned = true;

	return rc;
}

int DSRequestHardware(threadEP_t func, void* userData, hwreqfunc_t reqFunc)
{
	if (!hwReqFunc) return -1;
	hwreqfunc_t oldFunc = hwReqFunc;
	hwReqFunc = reqFunc;
	int rc = oldFunc(func, userData);
	hwReqFunc = oldFunc;
	return rc;
}

void _KeybdKey(int key)
{
	if (stdioRead && key > 0)
		putchar(key);
}

volatile static bool _hasExited = false;
volatile static int _rc = 0;

void _CheckExit()
{
	if (_hasExited)
		exit(_rc);
}

void IoInitStreams();
void KeInitSystemInfo();

void KeSystemError();

static void error_die()
{
	iprintf("\nPress START to exit...\n");
	for (;;)
	{
		swiWaitForVBlank();
		if (keysDown() & KEY_START)
		{
			_rc = 1;
			_hasExited = true;
		}
	}
}

#define ANSIESC_RED "\x1b[31;1m"
#define ANSIESC_GREEN "\x1b[32;1m"
#define ANSIESC_YELLOW "\x1b[33;1m"
#define ANSIESC_DEFAULT "\x1b[39;1m"

#define MSG_OK   ANSIESC_GREEN "  OK\n" ANSIESC_DEFAULT
#define MSG_OK2  ANSIESC_YELLOW "  OK\n\n" ANSIESC_DEFAULT
#define WARNING ANSIESC_YELLOW "WARNING: " ANSIESC_DEFAULT
#define MSG_FAIL ANSIESC_RED "FAIL\n\n" ANSIESC_DEFAULT

vu32* g_fatCwdClusterPtr;

int main()
{
	videoInit();
	consoleDebugInit(DebugDevice_CONSOLE);
	
	setExceptionHandler(KeSystemError);
	SystemVectors.reset = (u32) __ResetHandler;
	SystemVectors.swi = (u32) __SWIHandler;
	setVectorBase(0);
	LdrModuleListInit();
	IoInitStreams();
	installFeOSFIFO();
	KeInitSystemInfo();

	iprintf(
		"\n  FeOS kernel v" FEOS_VERSION_TEXT "\n"
		"   by FeOS Team, 2010-13\n");

#ifdef DEBUG
	iprintf(ANSIESC_YELLOW "  **DEBUG BUILD**\n");
	iprintf("  Built: " __DATE__ ", " __TIME__ "\n" ANSIESC_DEFAULT);
#endif
	iprintf("\nInitializing filesystem... ");
	if (!fatInitDefault())
	{
		iprintf(
			MSG_FAIL
			"Make sure you have DLDI patched\n"
			"the kernel binary. If the issue\n"
			"persists, try using HBMenu.\n\n"
			"  http://devkitpro.org/hbmenu\n"
			"  http://feos.mtheall.com/forum\n");
		error_die();
	}
	g_fatCwdClusterPtr = (vu32*) _FAT_getCwdClusterPtr("/");
	KeInitDefaultExecStatus();
	ThrInit();
	IoInstallThunks();
	iprintf(MSG_OK);

	iprintf("Initializing user mode...  ");
	KeInitUserMode();
	KeEnterUserMode();
	iprintf(MSG_OK);

#ifdef DEBUG
	iprintf("Loading debug library...   ");
	module_t hCxxLib = LdrLoadModule("feoscxx");
	if (!hCxxLib)
	{
		iprintf(
			MSG_FAIL
			"The following file is missing\n"
			"or it may have been corrupted:\n"
			"  /data/FeOS/lib/feoscxx.fx2\n");
		error_die();
	}
	iprintf(MSG_OK);
#endif

	iprintf("Startup...\n\n");

	const char* argv[] = { "cmd", ":startup", NULL };

	_rc = LdrExecuteArgv(2, argv);
	if (_rc < 0)
	{
		iprintf(
			MSG_FAIL
			"The following file is missing\n"
			"or it may have been corrupted:\n"
			"  /data/FeOS/bin/cmd.fx2\n");
		error_die();
	}
	_hasExited = true;

	for(;;) DSWaitForVBlankRaw();
}
