//
// FeOS Standard Library
// feosdsapi.h
//     FeOS DS-specific/libnds functions
//

#pragma once

typedef struct { int x, y; } styluspos_t;
typedef word_t keys_t;

// libnds compatibility defines
#define DC_FlushRange      FeOS_DataCacheFlush
#define DC_FlushAll        FeOS_DataCacheFlushAll
#define IC_InvalidateRange FeOS_InstrCacheInvalidate
#define IC_InvalidateAll   FeOS_InstrCacheInvalidateAll
#define keysUpdate()       ((void)0)
#define keysDown           FeOS_GetKeysDown
#define keysHeld           FeOS_GetKeysHeld
#define keysUp             FeOS_GetKeysUp
#define keysDownRepeat     FeOS_GetKeysDownRepeat
#define keysSetRepeat      FeOS_SetKeyRepeat
#define irqSet             FeOS_SetInterrupt
#define irqEnable          FeOS_IrqEnable
#define irqDisable         FeOS_IrqDisable
#define swiWaitForVBlank   FeOS_WaitForVBlank
#define timerTick          FeOS_TimerTick
#define timerStop          FeOS_TimerStop
#define fifoSetDatamsgHandler FeOS_FifoSetDatamsgHandler
#define fifoSetValue32Handler FeOS_FifoSetValue32Handler
#define fifoSetAddressHandler FeOS_FifoSetAddressHandler
#define fifoSendAddress       FeOS_FifoSendAddress
#define fifoSendValue32       FeOS_FifoSendValue32
#define fifoSendDatamsg       FeOS_FifoSendDatamsg
#define fifoCheckAddress      FeOS_FifoCheckAddress
#define fifoCheckValue32      FeOS_FifoCheckValue32
#define fifoCheckDatamsg      FeOS_FifoCheckDatamsg
#define fifoGetAddress        FeOS_FifoGetAddress
#define fifoGetValue32        FeOS_FifoGetValue32
#define fifoGetDatamsg        FeOS_FifoGetDatamsg

enum
{
	KEY_A      = BIT(0),  // A button
	KEY_B      = BIT(1),  // B button
	KEY_SELECT = BIT(2),  // SELECT button
	KEY_START  = BIT(3),  // START button
	KEY_RIGHT  = BIT(4),  // RIGHT button
	KEY_LEFT   = BIT(5),  // LEFT button
	KEY_UP     = BIT(6),  // UP button
	KEY_DOWN   = BIT(7),  // DOWN button
	KEY_R      = BIT(8),  // R button
	KEY_L      = BIT(9),  // L button
	KEY_X      = BIT(10), // X button
	KEY_Y      = BIT(11), // Y button
	KEY_TOUCH  = BIT(12)  // Touchscreen pen down
};

enum
{
	IRQ_VBLANK = BIT(0), // Vertical blank
	IRQ_HBLANK = BIT(1), // Horizontal blank
	IRQ_TIMER0 = BIT(3), // Timer 0
	IRQ_TIMER1 = BIT(4), // Timer 1
	IRQ_TIMER2 = BIT(5), // Timer 2
	IRQ_TIMER3 = BIT(6), // Timer 3
};

enum
{
	MODE_DIRECT = 0,
	MODE_CONSOLE = 1
};

#define BG_PALETTE         ((hword_t*) 0x05000000)
#define BG_PALETTE_SUB     ((hword_t*) 0x05000400)
#define SPRITE_PALETTE     ((hword_t*) 0x05000200)
#define SPRITE_PALETTE_SUB ((hword_t*) 0x05000600)

#define VRAM_A ((hword_t*) 0x6800000)
#define VRAM_B ((hword_t*) 0x6820000)
#define VRAM_C ((hword_t*) 0x6840000)
#define VRAM_D ((hword_t*) 0x6860000)
#define VRAM_E ((hword_t*) 0x6880000)
#define VRAM_F ((hword_t*) 0x6890000)
#define VRAM_G ((hword_t*) 0x6894000)
#define VRAM_H ((hword_t*) 0x6898000)
#define VRAM_I ((hword_t*) 0x68A0000)

#define RGB15(r,g,b)    ((r)|((g)<<5)|((b)<<10))
#define RGB5(r,g,b)     ((r)|((g)<<5)|((b)<<10))
#define RGB8(r,g,b)     (((r)>>3)|(((g)>>3)<<5)|(((b)>>3)<<10))
#define ARGB16(a,r,g,b) (((a)<<15)|(r)|((g)<<5)|((b)<<10))

#define VRAM_OFFSET(n) ((n) << 3)

enum
{
	VRAM_A_LCD                    = 0,
	VRAM_A_MAIN_BG                = 1,
	VRAM_A_MAIN_BG_0x06000000     = 1 | VRAM_OFFSET(0),
	VRAM_A_MAIN_BG_0x06020000     = 1 | VRAM_OFFSET(1),
	VRAM_A_MAIN_BG_0x06040000     = 1 | VRAM_OFFSET(2),
	VRAM_A_MAIN_BG_0x06060000     = 1 | VRAM_OFFSET(3),
	VRAM_A_MAIN_SPRITE            = 2,
	VRAM_A_MAIN_SPRITE_0x06400000 = 2 | VRAM_OFFSET(0),
	VRAM_A_MAIN_SPRITE_0x06420000 = 2 | VRAM_OFFSET(1),
	VRAM_A_TEXTURE                = 3,
	VRAM_A_TEXTURE_SLOT0          = 3 | VRAM_OFFSET(0),
	VRAM_A_TEXTURE_SLOT1          = 3 | VRAM_OFFSET(1),
	VRAM_A_TEXTURE_SLOT2          = 3 | VRAM_OFFSET(2),
	VRAM_A_TEXTURE_SLOT3          = 3 | VRAM_OFFSET(3)
};

enum
{
	VRAM_B_LCD                    = 0,
	VRAM_B_MAIN_BG                = 1 | VRAM_OFFSET(1),
	VRAM_B_MAIN_BG_0x06000000     = 1 | VRAM_OFFSET(0),
	VRAM_B_MAIN_BG_0x06020000     = 1 | VRAM_OFFSET(1),
	VRAM_B_MAIN_BG_0x06040000     = 1 | VRAM_OFFSET(2),
	VRAM_B_MAIN_BG_0x06060000     = 1 | VRAM_OFFSET(3),
	VRAM_B_MAIN_SPRITE            = 2,
	VRAM_B_MAIN_SPRITE_0x06400000 = 2 | VRAM_OFFSET(0),
	VRAM_B_MAIN_SPRITE_0x06420000 = 2 | VRAM_OFFSET(1),
	VRAM_B_TEXTURE                = 3 | VRAM_OFFSET(1),
	VRAM_B_TEXTURE_SLOT0          = 3 | VRAM_OFFSET(0),
	VRAM_B_TEXTURE_SLOT1          = 3 | VRAM_OFFSET(1),
	VRAM_B_TEXTURE_SLOT2          = 3 | VRAM_OFFSET(2),
	VRAM_B_TEXTURE_SLOT3          = 3 | VRAM_OFFSET(3)
};

enum
{
	VRAM_C_LCD                = 0,
	VRAM_C_MAIN_BG            = 1 | VRAM_OFFSET(2),
	VRAM_C_MAIN_BG_0x06000000 = 1 | VRAM_OFFSET(0),
	VRAM_C_MAIN_BG_0x06020000 = 1 | VRAM_OFFSET(1),
	VRAM_C_MAIN_BG_0x06040000 = 1 | VRAM_OFFSET(2),
	VRAM_C_MAIN_BG_0x06060000 = 1 | VRAM_OFFSET(3),
	VRAM_C_ARM7               = 2,
	VRAM_C_ARM7_0x06000000    = 2 | VRAM_OFFSET(0),
	VRAM_C_ARM7_0x06020000    = 2 | VRAM_OFFSET(1),
	VRAM_C_SUB_BG             = 4,
	VRAM_C_SUB_BG_0x06200000  = 4 | VRAM_OFFSET(0),
	VRAM_C_TEXTURE            = 3 | VRAM_OFFSET(2),
	VRAM_C_TEXTURE_SLOT0      = 3 | VRAM_OFFSET(0),
	VRAM_C_TEXTURE_SLOT1      = 3 | VRAM_OFFSET(1),
	VRAM_C_TEXTURE_SLOT2      = 3 | VRAM_OFFSET(2),
	VRAM_C_TEXTURE_SLOT3      = 3 | VRAM_OFFSET(3)
};

enum
{
	VRAM_D_LCD                = 0,
	VRAM_D_MAIN_BG            = 1 | VRAM_OFFSET(3),
	VRAM_D_MAIN_BG_0x06000000 = 1 | VRAM_OFFSET(0),
	VRAM_D_MAIN_BG_0x06020000 = 1 | VRAM_OFFSET(1),
	VRAM_D_MAIN_BG_0x06040000 = 1 | VRAM_OFFSET(2),
	VRAM_D_MAIN_BG_0x06060000 = 1 | VRAM_OFFSET(3),
	VRAM_D_ARM7               = 2 | VRAM_OFFSET(1),
	VRAM_D_ARM7_0x06000000    = 2 | VRAM_OFFSET(0),
	VRAM_D_ARM7_0x06020000    = 2 | VRAM_OFFSET(1),
	VRAM_D_SUB_SPRITE         = 4,
	VRAM_D_TEXTURE            = 3 | VRAM_OFFSET(3),
	VRAM_D_TEXTURE_SLOT0      = 3 | VRAM_OFFSET(0),
	VRAM_D_TEXTURE_SLOT1      = 3 | VRAM_OFFSET(1),
	VRAM_D_TEXTURE_SLOT2      = 3 | VRAM_OFFSET(2),
	VRAM_D_TEXTURE_SLOT3      = 3 | VRAM_OFFSET(3)
};

enum
{
	VRAM_E_LCD            = 0,
	VRAM_E_MAIN_BG        = 1,
	VRAM_E_MAIN_SPRITE    = 2,
	VRAM_E_TEX_PALETTE    = 3,
	VRAM_E_BG_EXT_PALETTE = 4,
};

enum
{
	VRAM_F_LCD                    = 0,
	VRAM_F_MAIN_BG                = 1,
	VRAM_F_MAIN_BG_0x06000000     = 1 | VRAM_OFFSET(0),
	VRAM_F_MAIN_BG_0x06004000     = 1 | VRAM_OFFSET(1),
	VRAM_F_MAIN_BG_0x06010000     = 1 | VRAM_OFFSET(2),
	VRAM_F_MAIN_BG_0x06014000     = 1 | VRAM_OFFSET(3),
	VRAM_F_MAIN_SPRITE            = 2,
	VRAM_F_MAIN_SPRITE_0x06400000 = 2 | VRAM_OFFSET(0),
	VRAM_F_MAIN_SPRITE_0x06404000 = 2 | VRAM_OFFSET(1),
	VRAM_F_MAIN_SPRITE_0x06410000 = 2 | VRAM_OFFSET(2),
	VRAM_F_MAIN_SPRITE_0x06414000 = 2 | VRAM_OFFSET(3),
	VRAM_F_TEX_PALETTE            = 3,
	VRAM_F_TEX_PALETTE_SLOT0      = 3 | VRAM_OFFSET(0),
	VRAM_F_TEX_PALETTE_SLOT1      = 3 | VRAM_OFFSET(1),
	VRAM_F_TEX_PALETTE_SLOT4      = 3 | VRAM_OFFSET(2),
	VRAM_F_TEX_PALETTE_SLOT5      = 3 | VRAM_OFFSET(3),
	VRAM_F_BG_EXT_PALETTE         = 4,
	VRAM_F_BG_EXT_PALETTE_SLOT01  = 4 | VRAM_OFFSET(0),
	VRAM_F_BG_EXT_PALETTE_SLOT23  = 4 | VRAM_OFFSET(1),
	VRAM_F_SPRITE_EXT_PALETTE     = 5,
};

enum
{
	VRAM_G_LCD                    = 0,
	VRAM_G_MAIN_BG                = 1,
	VRAM_G_MAIN_BG_0x06000000     = 1 | VRAM_OFFSET(0),
	VRAM_G_MAIN_BG_0x06004000     = 1 | VRAM_OFFSET(1),
	VRAM_G_MAIN_BG_0x06010000     = 1 | VRAM_OFFSET(2),
	VRAM_G_MAIN_BG_0x06014000     = 1 | VRAM_OFFSET(3),
	VRAM_G_MAIN_SPRITE            = 2,
	VRAM_G_MAIN_SPRITE_0x06400000 = 2 | VRAM_OFFSET(0),
	VRAM_G_MAIN_SPRITE_0x06404000 = 2 | VRAM_OFFSET(1),
	VRAM_G_MAIN_SPRITE_0x06410000 = 2 | VRAM_OFFSET(2),
	VRAM_G_MAIN_SPRITE_0x06414000 = 2 | VRAM_OFFSET(3),
	VRAM_G_TEX_PALETTE            = 3,
	VRAM_G_TEX_PALETTE_SLOT0      = 3 | VRAM_OFFSET(0),
	VRAM_G_TEX_PALETTE_SLOT1      = 3 | VRAM_OFFSET(1),
	VRAM_G_TEX_PALETTE_SLOT4      = 3 | VRAM_OFFSET(2),
	VRAM_G_TEX_PALETTE_SLOT5      = 3 | VRAM_OFFSET(3),
	VRAM_G_BG_EXT_PALETTE         = 4,
	VRAM_G_BG_EXT_PALETTE_SLOT01  = 4 | VRAM_OFFSET(0),
	VRAM_G_BG_EXT_PALETTE_SLOT23  = 4 | VRAM_OFFSET(1),
	VRAM_G_SPRITE_EXT_PALETTE     = 5,
};

enum
{
	VRAM_H_LCD                = 0,
	VRAM_H_SUB_BG             = 1,
	VRAM_H_SUB_BG_EXT_PALETTE = 2,
};

enum
{
	VRAM_I_LCD                    = 0,
	VRAM_I_SUB_BG_0x06208000      = 1,
	VRAM_I_SUB_SPRITE             = 2,
	VRAM_I_SUB_SPRITE_EXT_PALETTE = 3,
};

typedef hword_t dspal_t[256];
typedef dspal_t dsextpal_t[16];

#define VRAM_E_EXT_PALETTE ((dsextpal_t*) VRAM_E)
#define VRAM_F_EXT_PALETTE ((dsextpal_t*) VRAM_F)
#define VRAM_G_EXT_PALETTE ((dsextpal_t*) VRAM_G)
#define VRAM_H_EXT_PALETTE ((dsextpal_t*) VRAM_H)
#define VRAM_F_EXT_SPR_PALETTE ((dspal_t*) VRAM_F)
#define VRAM_G_EXT_SPR_PALETTE ((dspal_t*) VRAM_G)
#define VRAM_I_EXT_SPR_PALETTE ((dspal_t*) VRAM_I)

enum
{
	MODE_0_2D = 0x10000,
	MODE_1_2D = 0x10001,
	MODE_2_2D = 0x10002,
	MODE_3_2D = 0x10003,
	MODE_4_2D = 0x10004,
	MODE_5_2D = 0x10005,
	MODE_6_2D = 0x10006,
	MODE_0_3D = 0x10000 | BIT(8) | BIT(3),
	MODE_1_3D = 0x10001 | BIT(8) | BIT(3),
	MODE_2_3D = 0x10002 | BIT(8) | BIT(3),
	MODE_3_3D = 0x10003 | BIT(8) | BIT(3),
	MODE_4_3D = 0x10004 | BIT(8) | BIT(3),
	MODE_5_3D = 0x10005 | BIT(8) | BIT(3),
	MODE_6_3D = 0x10006 | BIT(8) | BIT(3),
	MODE_FIFO = 3 << 16,
	MODE_FB0  = 0x00020000,
	MODE_FB1  = 0x00060000,
	MODE_FB2  = 0x000A0000,
	MODE_FB3  = 0x000E0000,

	DISPLAY_WIN0_ON = BIT(13),
	DISPLAY_WIN1_ON = BIT(14),
	DISPLAY_SPR_WIN_ON = BIT(15),
};

keys_t FeOS_GetKeysDown();
keys_t FeOS_GetKeysHeld();
keys_t FeOS_GetKeysUp();
keys_t FeOS_GetKeysDownRepeat();
void FeOS_SetKeyRepeat(byte_t, byte_t);
void FeOS_GetStylusPos(styluspos_t*);

fp_t FeOS_SetInterrupt(word_t, fp_t);
word_t FeOS_CheckPendingIRQs();
void FeOS_WaitForIRQ(word_t);
void FeOS_IrqEnable(word_t);
void FeOS_IrqDisable(word_t);

void FeOS_TimerWrite(int, word_t);
hword_t FeOS_TimerTick(int);

static inline void FeOS_TimerStop(int timer)
{
	FeOS_TimerWrite(timer, 0);
}

void FeOS_ConsoleMode();
void FeOS_DirectMode();
int FeOS_GetMode();

void dmaCopyWords(int, const void*, void*, word_t);
void dmaCopyHalfWords(int, const void*, void*, word_t);
void dmaFillWords(word_t, void*, word_t);
void dmaFillHalfWords(word_t, void*, word_t);
void* memUncached(void*);
void* memCached(void*);
static inline void dmaCopy(const void* in, void* out, word_t size)
{
	dmaCopyHalfWords(3, in, out, size);
}

word_t vramSetPrimaryBanks(word_t, word_t, word_t, word_t);
word_t vramSetBanks_EFG(word_t, word_t, word_t);
void vramRestorePrimaryBanks(word_t);
void vramRestoreBanks_EFG(word_t);
void vramSetBankA(word_t);
void vramSetBankB(word_t);
void vramSetBankC(word_t);
void vramSetBankD(word_t);
void vramSetBankE(word_t);
void vramSetBankF(word_t);
void vramSetBankG(word_t);
void vramSetBankH(word_t);
void vramSetBankI(word_t);

void videoSetMode(word_t);
void videoSetModeSub(word_t);
word_t videoGetMode();
word_t videoGetModeSub();
bool video3DEnabled();
void setBrightness(int screen, int level);
void lcdSwap();
void lcdMainOnTop();
void lcdMainOnBottom();
