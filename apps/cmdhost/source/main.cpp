#include "common.h"
#include "font_bin.h"
#include "caret.h"
#include "icon.h"
#include "selected.h"

IGuiManager* g_guiManager;

static const color_t conPal[16] =
{
	RGB15(0,0,0),    // 30 normal black
	RGB15(15,0,0),   // 31 normal red
	RGB15(0,15,0),   // 32 normal green
	RGB15(15,15,0),  // 33 normal yellow
	RGB15(0,0,15),   // 34 normal blue
	RGB15(15,0,15),  // 35 normal magenta
	RGB15(0,15,15),  // 36 normal cyan
	RGB15(24,24,24), // 37 normal white
	RGB15(15,15,15), // 40 bright black
	RGB15(31,0,0),   // 41 bright red
	RGB15(0,31,0),   // 42 bright green
	RGB15(31,31,0),  // 43 bright yellow
	RGB15(0,0,31),   // 44 bright blue
	RGB15(31,0,31),  // 45 bright magenta
	RGB15(0,31,31),  // 46 bright cyan
	RGB15(31,31,31)  // 47 & 39 bright white
};

#define ANSIESC_RED "\x1b[31;1m"
#define ANSIESC_GREEN "\x1b[32;1m"
#define ANSIESC_YELLOW "\x1b[33;1m"
#define ANSIESC_DEFAULT "\x1b[39;1m"

static inline void writeRow(u32*& p, int v)
{
	register word_t t[4];
	for (int i = 0; i < 4; i ++)
	{
		t[i] = (v >> (i*2)) & 3;
		t[i] = (t[i] & 1) | ((t[i] >> 1) << 4);
	}
	*p++ = t[3] | (t[2] << 8) | (t[1] << 16) | (t[0] << 24);
}

class ConsoleHostApp : public CApplication
{
	thread_t hChildThread;
	CConsole oCon;
	KeyboardPtr oKbd;
	u16* conMap;
	SpriteEntry *caretSpr, *selSpr;
	int blinkTimer;
	static stream_t hostStreamSt;
	FILE *fwritehook, *freadhook;
	bool bIsInteractive, bFinished;

	int bgKeybd, bgBmp;
	color_t* bmpBuf;

#define _X(x) ((ConsoleHostApp*)(x))

	static ssize_t Write(void* pThis, const char* buf, size_t len)
	{
		FeOS_Yield();
		return _X(pThis)->oCon.print(buf, (int)len);
	}

	static ssize_t Read(void* pThis, char* buf, size_t len)
	{
		FeOS_Yield();
		return _X(pThis)->oKbd->SyncRead(buf, len);
	}

public:
	ConsoleHostApp() : blinkTimer(0), bFinished(false)
	{
		// Set the title of our application
		SetTitle("Command Prompt");

		// Set the flags
		SetFlags(AppFlags::UsesSelect);

		// Set the icon
		SetIcon((color_t*)iconBitmap);
	}

	~ConsoleHostApp()
	{
		FeOS_FreeThread(hChildThread);
		fclose(freadhook);
		fclose(fwritehook);
	}

	bool Initialize(int argc, const char* argv[])
	{
		oKbd = g_guiManager->CreateKeyboard();
		if (!oKbd)
			return false;

		if (argc == 0)
		{
			static const char* stock_argv[] = { "cmd", nullptr };
			bIsInteractive = true;
			argc = 1;
			argv = stock_argv;
		} else
		{
			bIsInteractive = false;

			const char* mainFile = argv[0];
			int mFlen = strlen(mainFile);
			if (mFlen > 4 && stricmp(mainFile + mFlen - 4, ".cmd") == 0)
			{
				argc ++;
				argv --;
				argv[0] = "cmd";
			}
		}

		fwritehook = FeOS_OpenStream(&hostStreamSt, this);
		if (!fwritehook) return false;

		freadhook = FeOS_OpenStream(&hostStreamSt, this);
		if (!freadhook) return false;

		setvbuf(fwritehook, NULL, _IONBF, 0);
		setvbuf(freadhook, NULL, _IOLBF, 256);

		FILE *fprevhook[3];
		fprevhook[0] = FeOS_SetStdin(freadhook);
		fprevhook[1] = FeOS_SetStdout(fwritehook);
		fprevhook[2] = FeOS_SetStderr(fwritehook);
		
		hChildThread = FeOS_CreateProcess(argc, argv);

		FeOS_SetStdin(fprevhook[0]);
		FeOS_SetStdout(fprevhook[1]);
		FeOS_SetStderr(fprevhook[2]);

		return hChildThread != nullptr;
	}

	void OnActivate()
	{
		//-------------------------
		// Main screen

		videoSetMode(MODE_0_2D);
		int conBg = bgInit(0, BgType_Text4bpp, BgSize_T_256x256, 5, 0);
		conMap = bgGetMapPtr(conBg);
		u32* tilePtr = (u32*) bgGetGfxPtr(conBg);
		for (int i = 0; i < font_bin_size; i ++)
			writeRow(tilePtr, font_bin[i]);

		oamInit(&oamMain, SpriteMapping_1D_128, false);
		dmaCopy(caretTiles, SPRITE_GFX, caretTilesLen);
		dmaCopy(caretPal, SPRITE_PALETTE, caretPalLen);

		oamSet(&oamMain, 0, 0, 0, 0, 0, SpriteSize_8x8, SpriteColorFormat_16Color, SPRITE_GFX, -1, false, false, false, false, false);
		caretSpr = FeOS_GetOAMMemory(&oamMain);

		caretSpr->isHidden = true;

		for (int i = 0; i < 16; i ++)
			BG_PALETTE[i*16 + 1] = conPal[i];

		//-------------------------
		// Sub screen

		videoSetModeSub(MODE_3_2D);
		int bgKeybd = bgInitSub(0, BgType_Text8bpp, BgSize_T_256x256, 7, 0);
		int bgBmp = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 1, 0);
		bgSetPriority(bgKeybd, 3);
		bgSetPriority(bgBmp, 0);

		oamInit(&oamSub, SpriteMapping_1D_128, false);
		dmaCopy(selectedTiles, SPRITE_GFX_SUB, selectedTilesLen);
		dmaCopy(selectedPal, SPRITE_PALETTE_SUB, selectedPalLen);

		oamSet(&oamSub, 0, 0, 0, 0, 0, SpriteSize_8x8, SpriteColorFormat_16Color, SPRITE_GFX_SUB, -1, false, false, false, false, false);
		selSpr = FeOS_GetOAMMemory(&oamSub);

		selSpr->isHidden = true;

		auto kbdGfx = oKbd->GetGraphics();
		dmaCopy(kbdGfx->gfxData, bgGetGfxPtr(bgKeybd), MemChunk_GetSize(kbdGfx->gfxData));
		dmaCopy(kbdGfx->mapData, bgGetMapPtr(bgKeybd), MemChunk_GetSize(kbdGfx->mapData));
		dmaCopy(kbdGfx->palData, BG_PALETTE_SUB,       MemChunk_GetSize(kbdGfx->palData));

		bmpBuf = bgGetGfxPtr(bgBmp);
		oKbd->SetDrawBuffer(bmpBuf);
	}

	void OnVBlank()
	{
		if (!FeOS_IsThreadActive(hChildThread) && !bFinished) do
		{
			bFinished = true;

			int rc = FeOS_GetThreadRC(hChildThread);
			if (rc != 0)
			{
				fprintf(fwritehook, "\nThe application exited with a\nfailure code of %d.\nPress START to close...\n", rc);
				break;
			}

			if (bIsInteractive)
			{
				Close();
				return;
			}

			fprintf(fwritehook, "\nThe application just exited.\nPress START to close...\n");
		} while(0);

		word_t kDown = keysDown();

		if (bFinished && kDown & KEY_START)
		{
			Close();
			return;
		}

		bool bIsReading = oKbd->InSyncRead();

		oCon.render(conMap);
		setBrightness(2, bIsReading ? 0 : -8);

		blinkTimer ++;
		if (blinkTimer >= 30) blinkTimer = 0;
		caretSpr->isHidden = !!(blinkTimer > 15);
		caretSpr->x = oCon.x * 8;
		caretSpr->y = oCon.y * 8;

		if (kDown & KEY_TOUCH)
		{
			TouchPos tp;
			int ret = oKbd->OnClick(tp);
			if (ret >= 0)
			{
				if (bIsReading && ret != 0)
					oCon.print(ret);

				selSpr->x = tp.px - 4;
				selSpr->y = tp.py - 4;
				selSpr->isHidden = false;
			} else if (tp.py >= (192-8))
				g_guiManager->SwitchTo(nullptr);
		} else if (keysUp() & KEY_TOUCH)
			selSpr->isHidden = true;
	}
};

stream_t ConsoleHostApp::hostStreamSt =
{
	NULL,
	NULL,
	ConsoleHostApp::Write,
	ConsoleHostApp::Read,
	NULL
};

int main(int argc, const char* argv[])
{
	g_guiManager = GetGuiManagerChecked();

	ConsoleHostApp app;
	if (!app.Initialize(argc - 1, argv + 1))
		return 0;
	g_guiManager->RunApplication(&app);

	return 0;
}
