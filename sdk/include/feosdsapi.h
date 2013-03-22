//
// FeOS Standard Library
// feosdsapi.h
//     FeOS DS-specific/libnds functions
//

#pragma once

/** @file feosdsapi.h
 *  \brief FeOS DS-specific/libnds functions
 */

/** @addtogroup api_ds FeOS libnds-like basic DS API
 *  \brief FeOS libnds-like general DS hardware access functionality.
 *  \code
 *  #include <feos.h>
 *  \endcode
 *  It is strongly recommended to use <a href="http://mtheall.com/banks.html#A=MBG0&B=MOBJ0&C=SBG&D=SOBJ">
 *  mtheall's VRAM Bank tool</a> in order to generate a VRAM bank layout.
 *  @{
 */

//! \headerfile feos.h <feos.h>
//! \brief Structure for FeOS_GetStylusPos().
typedef struct { int x, y; } styluspos_t;
typedef word_t keys_t;

// libnds-compatible type defines
typedef byte_t u8; //!< libnds unsigned 8-bit integer type
typedef char_t s8; //!< libnds signed 8-bit integer type
typedef hword_t u16; //!< libnds unsigned 16-bit integer type
typedef short_t s16; //!< libnds signed 16-bit integer type
typedef word_t u32; //!< libnds unsigned 32-bit integer type
typedef long_t s32; //!< libnds signed 32-bit integer type
typedef dword_t u64; //!< libnds unsigned 64-bit integer type
typedef dlong_t s64; //!< libnds signed 64-bit integer type

typedef volatile byte_t vu8; //!< libnds volatile unsigned 8-bit integer type
typedef volatile char_t vs8; //!< libnds volatile signed 8-bit integer type
typedef volatile hword_t vu16; //!< libnds volatile unsigned 16-bit integer type
typedef volatile short_t vs16; //!< libnds volatile signed 16-bit integer type
typedef volatile word_t vu32; //!< libnds volatile unsigned 32-bit integer type
typedef volatile long_t vs32; //!< libnds volatile signed 32-bit integer type
typedef volatile dword_t vu64; //!< libnds volatile unsigned 64-bit integer type
typedef volatile dlong_t vs64; //!< libnds volatile signed 64-bit integer type

typedef byte_t uint8; //!< libnds unsigned 8-bit integer type
typedef char_t int8; //!< libnds signed 8-bit integer type
typedef hword_t uint16; //!< libnds unsigned 16-bit integer type
typedef short_t int16; //!< libnds signed 16-bit integer type
typedef word_t uint32; //!< libnds unsigned 32-bit integer type
typedef long_t int32; //!< libnds signed 32-bit integer type
typedef dword_t uint64; //!< libnds unsigned 64-bit integer type
typedef dlong_t int64; //!< libnds signed 64-bit integer type

typedef volatile uint8 vuint8; //!< libnds volatile unsigned 8-bit integer type
typedef volatile int8 vint8; //!< libnds volatile signed 8-bit integer type
typedef volatile uint16 vuint16; //!< libnds volatile unsigned 16-bit integer type
typedef volatile int16 vint16; //!< libnds volatile signed 16-bit integer type
typedef volatile uint32 vuint32; //!< libnds volatile unsigned 32-bit integer type
typedef volatile int32 vint32; //!< libnds volatile signed 32-bit integer type
typedef volatile dword_t vuint64; //!< libnds volatile unsigned 64-bit integer type
typedef volatile dlong_t vint64; //!< libnds volatile signed 64-bit integer type

// libnds compatibility defines
#define timerTick FeOS_TimerTick //!< libnds alias for FeOS_TimerTick()
#define timerStop FeOS_TimerStop //!< libnds alias for FeOS_TimerStop()

//! \brief Screen resolution constants
enum
{
	SCREEN_WIDTH = 256, SCREEN_HEIGHT = 192
};

//! \brief DS button bitmask
enum
{
	KEY_A      = BIT(0),  //!< A button
	KEY_B      = BIT(1),  //!< B button
	KEY_SELECT = BIT(2),  //!< SELECT button
	KEY_START  = BIT(3),  //!< START button
	KEY_RIGHT  = BIT(4),  //!< RIGHT button
	KEY_LEFT   = BIT(5),  //!< LEFT button
	KEY_UP     = BIT(6),  //!< UP button
	KEY_DOWN   = BIT(7),  //!< DOWN button
	KEY_R      = BIT(8),  //!< R button
	KEY_L      = BIT(9),  //!< L button
	KEY_X      = BIT(10), //!< X button
	KEY_Y      = BIT(11), //!< Y button
	KEY_TOUCH  = BIT(12), //!< Touchscreen pen down
	KEY_LID    = BIT(13)  //!< Hinge state
};

//! \brief DS interrupt bitmask
enum
{
	IRQ_VBLANK = BIT(0), //!< Vertical blank
	IRQ_HBLANK = BIT(1), //!< Horizontal blank
	IRQ_TIMER0 = BIT(3), //!< Timer 0
	IRQ_TIMER1 = BIT(4), //!< Timer 1
	IRQ_TIMER2 = BIT(5), //!< Timer 2
	IRQ_TIMER3 = BIT(6), //!< Timer 3
};

//! \brief FeOS operation modes
enum
{
	MODE_DIRECT = 0, //!< Direct mode
	MODE_CONSOLE = 1 //!< Console mode
};

#define BG_PALETTE         ((hword_t*) 0x05000000) //!< DS engine A background palette memory
#define BG_PALETTE_SUB     ((hword_t*) 0x05000400) //!< DS engine B background palette memory
#define SPRITE_PALETTE     ((hword_t*) 0x05000200) //!< DS engine A sprite palette memory
#define SPRITE_PALETTE_SUB ((hword_t*) 0x05000600) //!< DS engine B sprite palette memory

#define BG_GFX             ((hword_t*)0x6000000) //!< DS engine A background graphics memory
#define BG_GFX_SUB         ((hword_t*)0x6200000) //!< DS engine B background graphics memory
#define SPRITE_GFX         ((hword_t*)0x6400000) //!< DS engine A sprite graphics memory
#define SPRITE_GFX_SUB     ((hword_t*)0x6600000) //!< DS engine B sprite graphics memory

#define VRAM_A ((hword_t*) 0x6800000) //!< DS VRAM bank A memory
#define VRAM_B ((hword_t*) 0x6820000) //!< DS VRAM bank B memory
#define VRAM_C ((hword_t*) 0x6840000) //!< DS VRAM bank C memory
#define VRAM_D ((hword_t*) 0x6860000) //!< DS VRAM bank D memory
#define VRAM_E ((hword_t*) 0x6880000) //!< DS VRAM bank E memory
#define VRAM_F ((hword_t*) 0x6890000) //!< DS VRAM bank F memory
#define VRAM_G ((hword_t*) 0x6894000) //!< DS VRAM bank G memory
#define VRAM_H ((hword_t*) 0x6898000) //!< DS VRAM bank H memory
#define VRAM_I ((hword_t*) 0x68A0000) //!< DS VRAM bank I memory

#define RGB15(r,g,b)    ((r)|((g)<<5)|((b)<<10)) //!< Converts 5-bit RGB components into a single 16-bit color (alpha bit off)
#define RGB5(r,g,b)     ((r)|((g)<<5)|((b)<<10)) //!< Converts 5-bit RGB components into a single 16-bit color (alpha bit off)
#define RGB8(r,g,b)     (((r)>>3)|(((g)>>3)<<5)|(((b)>>3)<<10)) //!< Converts 8-bit RGB components into a single 16-bit color (alpha bit off)
#define ARGB16(a,r,g,b) (((a)<<15)|(r)|((g)<<5)|((b)<<10)) //!< Converts 5-bit RGB components plus 1-bit alpha into a single 16-bit color

#define VRAM_OFFSET(n) ((n) << 3) //!< Macro for VRAM offset

//! \brief VRAM A operation modes
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

//! \brief VRAM B operation modes
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

//! \brief VRAM C operation modes
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

//! \brief VRAM D operation modes
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

//! \brief VRAM E operation modes
enum
{
	VRAM_E_LCD            = 0,
	VRAM_E_MAIN_BG        = 1,
	VRAM_E_MAIN_SPRITE    = 2,
	VRAM_E_TEX_PALETTE    = 3,
	VRAM_E_BG_EXT_PALETTE = 4,
};

//! \brief VRAM F operation modes
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

//! \brief VRAM G operation modes
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

//! \brief VRAM H operation modes
enum
{
	VRAM_H_LCD                = 0,
	VRAM_H_SUB_BG             = 1,
	VRAM_H_SUB_BG_EXT_PALETTE = 2,
};

//! \brief VRAM I operation modes
enum
{
	VRAM_I_LCD                    = 0,
	VRAM_I_SUB_BG_0x06208000      = 1,
	VRAM_I_SUB_SPRITE             = 2,
	VRAM_I_SUB_SPRITE_EXT_PALETTE = 3,
};

typedef hword_t dspal_t[256]; //!< DS palette data structure
typedef dspal_t dsextpal_t[16]; //!< DS extended palette data structure

#define VRAM_E_EXT_PALETTE ((dsextpal_t*) VRAM_E) //!< DS VRAM bank E memory as extended background palette
#define VRAM_F_EXT_PALETTE ((dsextpal_t*) VRAM_F) //!< DS VRAM bank F memory as extended background palette
#define VRAM_G_EXT_PALETTE ((dsextpal_t*) VRAM_G) //!< DS VRAM bank G memory as extended background palette
#define VRAM_H_EXT_PALETTE ((dsextpal_t*) VRAM_H) //!< DS VRAM bank H memory as extended background palette
#define VRAM_F_EXT_SPR_PALETTE ((dspal_t*) VRAM_F) //!< DS VRAM bank F memory as extended sprite palette
#define VRAM_G_EXT_SPR_PALETTE ((dspal_t*) VRAM_G) //!< DS VRAM bank G memory as extended sprite palette
#define VRAM_I_EXT_SPR_PALETTE ((dspal_t*) VRAM_I) //!< DS VRAM bank H memory as extended sprite palette

//! \brief DS video modes
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

//! \cond
#define PM_ARM9_DIRECT BIT(16)
//! \endcond

//! \brief DS hardware power bitmask
enum
{
	PM_SOUND_AMP        = BIT(0), //!< Power the sound hardware (needed to hear stuff in GBA mode too).
	PM_SOUND_MUTE       = BIT(1), //!< Mute the main speakers, headphone output will still work.
	PM_BACKLIGHT_BOTTOM = BIT(2), //!< Enable the bottom backlight if set.
	PM_BACKLIGHT_TOP    = BIT(3), //!< Enable the top backlight if set.
	PM_SYSTEM_PWR       = BIT(6), //!< Turn the power *off* if set.

	POWER_LCD       = PM_ARM9_DIRECT | BIT(0),  //!< Controls the power for both LCD screens.
	POWER_2D_A      = PM_ARM9_DIRECT | BIT(1),  //!< Controls the power for the main 2D core.
	POWER_MATRIX    = PM_ARM9_DIRECT | BIT(2),  //!< Controls the power for the 3D matrix.
	POWER_3D_CORE   = PM_ARM9_DIRECT | BIT(3),  //!< Controls the power for the main 3D core.
	POWER_2D_B      = PM_ARM9_DIRECT | BIT(9),  //!< Controls the power for the sub 2D core.
	POWER_SWAP_LCDS = PM_ARM9_DIRECT | BIT(15), //!< Controls which screen should use the main core.
	POWER_ALL_2D    = PM_ARM9_DIRECT | POWER_LCD | POWER_2D_A | POWER_2D_B,        //!< Power just 2D hardware.
	POWER_ALL       = PM_ARM9_DIRECT | POWER_ALL_2D | POWER_3D_CORE | POWER_MATRIX //!< Power everything.
};

//! \brief Retrieves a bitmask containing the keys that have just been pressed.
keys_t keysDown();
//! \brief Retrieves a bitmask containing the keys that are pressed.
keys_t keysHeld();
//! \brief Retrieves a bitmask containing the keys that have just been released.
keys_t keysUp();
//! \brief Retrieves a bitmask containing the keys that have just been pressed, accounting for auto-repeat.
keys_t keysDownRepeat();
//! \brief Configures the timing for key auto-repeat.
void keysSetRepeat(byte_t, byte_t);
//! \brief Retrieves the stylus position.
void FeOS_GetStylusPos(styluspos_t*);
//! \brief Scans the status of the keypad. Has no effect if AUTOUPD_KEYS is turned on.
void scanKeys();

// libnds-compat stylus access

//! \headerfile feos.h <feos.h>
//! \brief libnds touch position structure for touchRead().
typedef struct { int px, py; } touchPosition;

//! \brief Retrieves the stylus position (libnds-like version).
static inline void touchRead(touchPosition* tp)
{
	FeOS_GetStylusPos((styluspos_t*) tp);
}

//! \brief Registers a function to be called when the specified interrupt(s) is (are) fired.
//! \warning FeOS handles interrupt and FIFO callbacks in a cooperative way instead of
//! immediately executing the handlers. This means that callbacks will only be called
//! if and when an application uses IRQ waiting functions or FeOS_CheckPendingIRQs().
//! \returns The previous callback function.
fp_t irqSet(word_t, fp_t);

//! \brief Returns the function that handles a certain IRQ mask.
static inline fp_t irqGet(word_t mask)
{
	return irqSet(mask, (fp_t)0xFFFFFFFF);
}

//! \brief Checks for interrupts that may have been fired and runs any necessary callbacks.
//! \returns The interrupts that may have been fired.
word_t FeOS_CheckPendingIRQs();

//! \brief Waits for one of the specified interrupts to occur.
void FeOS_WaitForIRQ(word_t);
//! \brief Enables the specified interrupt(s).
void irqEnable(word_t);
//! \brief Disables the specified interrupt(s).
void irqDisable(word_t);
//! \brief Waits for any interrupt to occur. May return immediately if there were pending interrupts.
//! \returns The interrupt(s) that occured.
word_t FeOS_NextIRQ();

//! \brief Waits for the next VBlank to occur.
static inline void swiWaitForVBlank()
{
	FeOS_WaitForIRQ(IRQ_VBLANK);
}

#define DEFAULT_IRQFUNC ((irqWaitFunc_t)0) //!< Internal macro not for public consumption.
#define GET_IRQFUNC ((irqWaitFunc_t)0xFFFFFFFF) //!< Internal macro not for public consumption.
typedef void (*irqWaitFunc_t)(word_t); //!< Internal data type not for public consumption.

//! \brief Internal function not for public consumption.
irqWaitFunc_t FeOS_SetIRQWaitFunc(irqWaitFunc_t newFunc);

//! \brief Writes a value into a timer register.
void FeOS_TimerWrite(int, word_t);
//! \brief Reads a timer's counting value.
hword_t FeOS_TimerTick(int);

//! \brief Stops a timer.
static inline void FeOS_TimerStop(int timer)
{
	FeOS_TimerWrite(timer, 0);
}

//! \brief Switches to console mode. This should be the last thing done before exiting a direct mode program.
void FeOS_ConsoleMode();
//! \brief Switches to direct mode.
void FeOS_DirectMode();
//! \brief Returns the current operation mode.
int FeOS_GetMode();

//! \brief Internal data type not for public consumption.
typedef struct
{
	void (* directMode)();
	void (* consoleMode)();
	int (* getMode)();
} modeshim_t;

//! \brief Internal macro not for public consumption.
#define FEOS_GET_SHIM ((const modeshim_t*)~0)
//! \brief Internal function not for public consumption.
const modeshim_t* FeOS_ModeShim(const modeshim_t* pNewShim);

//! \brief Resets the video hardware to its default state after a FeOS_DirectMode() call.
void FeOS_VideoReset();

//! \brief Copies words using the specified DMA channel.
void dmaCopyWords(int, const void*, void*, word_t);
//! \brief Copies halfwords using the specified DMA channel.
void dmaCopyHalfWords(int, const void*, void*, word_t);
//! \brief Fills words using the specified DMA channel.
void dmaFillWords(word_t, void*, word_t);
//! \brief Fills halfwords sing the specified DMA channel.
void dmaFillHalfWords(word_t, void*, word_t);
//! \brief Returns the uncached mirror of the specified address.
void* memUncached(void*);
//! \brief Returns the cached mirror of the specified address.
void* memCached(void*);
//! \brief Copies data using DMA.
static inline void dmaCopy(const void* in, void* out, word_t size)
{
	dmaCopyHalfWords(3, in, out, size);
}

//! \brief Sets the operation mode for VRAM banks A, B, C and D (respectively) in one go.
//! \returns Mask for use with vramRestorePrimaryBanks().
word_t vramSetPrimaryBanks(word_t, word_t, word_t, word_t);
//! \brief Sets the operation mode for VRAM banks E, F and G (respectively) in one go.
//! \returns Mask for use with vramRestoreBanks_EFG().
word_t vramSetBanks_EFG(word_t, word_t, word_t);
//! \brief Restores the old operation mode for VRAM banks A, B, C and D.
void vramRestorePrimaryBanks(word_t);
//! \brief Restores the old operation mode for VRAM banks E, F and G.
void vramRestoreBanks_EFG(word_t);
//! \brief Sets the operation mode for VRAM bank A.
void vramSetBankA(word_t);
//! \brief Sets the operation mode for VRAM bank B.
void vramSetBankB(word_t);
//! \brief Sets the operation mode for VRAM bank C.
void vramSetBankC(word_t);
//! \brief Sets the operation mode for VRAM bank D.
void vramSetBankD(word_t);
//! \brief Sets the operation mode for VRAM bank E.
void vramSetBankE(word_t);
//! \brief Sets the operation mode for VRAM bank F.
void vramSetBankF(word_t);
//! \brief Sets the operation mode for VRAM bank G.
void vramSetBankG(word_t);
//! \brief Sets the operation mode for VRAM bank H.
void vramSetBankH(word_t);
//! \brief Sets the operation mode for VRAM bank I.
void vramSetBankI(word_t);

//! \brief Sets video engine A's operation mode.
void videoSetMode(word_t);
//! \brief Sets video engine B's operation mode.
void videoSetModeSub(word_t);
//! \brief Gets video engine A's operation mode.
word_t videoGetMode();
//! \brief Gets video engine B's operation mode.
word_t videoGetModeSub();
//! \brief Returns whether 3D is enabled.
bool video3DEnabled();
//! \brief Sets a screen's brightness level (-16..16).
void setBrightness(int screen, int level);
//! \brief Swaps the screens.
void lcdSwap();
//! \brief Places engine A on the top screen and engine B on the bottom screen.
void lcdMainOnTop();
//! \brief Places engine B on the top screen and engine A on the bottom screen.
void lcdMainOnBottom();
//! \brief Powers on the specified DS hardware.
void powerOn(int);
//! \brief Powers off the specified DS hardware.
void powerOff(int);

/** @} */
