#include "feos.h"
#include "fxe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void videoReset();
void InstallConThunks();
void InstallConDummy();

extern const byte_t __itcm_start[];
extern const byte_t __end__[];

static const char* registerNames[] =
{
	"r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
	"r8 ", "r9 ", "r10", "r11", "r12", "sp ", "lr ", "pc "
};

static inline bool between(word_t v, word_t lo, word_t hi)
{
	return (v >= lo) && (v < hi);
}

static PrintConsole oConSub;

typedef struct
{
	const char* name;
	instance_t module;
	word_t offset;
} part_t;

static bool resolveAddr(part_t* pPart, void* addr)
{
	if (!between((word_t)addr, 0x02000000, 0x03000000)) return false;
	addr = memCached(addr);
	if ((word_t)addr < (word_t)__end__)
	{
		pPart->name = "(kernel)";
		pPart->module = NULL;
		pPart->offset = (word_t)addr;
		return true;
	}
	instance_t module = FeOS_ModuleFromAddress(addr);
	if (!module) return false;

	pPart->name = GetRuntimeData(module)->name;
	pPart->module = module;
	pPart->offset = addr - module;
	return true;
}

static void printPartedAddr(const char* name, void* addr)
{
	iprintf("  %s: ", name);
	part_t oPart;
	if (resolveAddr(&oPart, addr))
		iprintf("%s%c0x%X\n", oPart.name, *oPart.name != '(' ? '+' : ' ', oPart.offset);
	else
		iprintf("--\n");
}

static void showRegMeanings()
{
	iprintf("\x1b[2J\x1b[5CRegister meanings\n\n");

	int i;
	for (i = 0; i < 16; i ++)
		printPartedAddr(registerNames[i], (void*)exceptionRegisters[i]);

	const char* modeStr = "(Unknown)";
	switch(*(u32*)0x02FFFD90 & 0xF)
	{
		case 0: modeStr = "User"; break;
		case 2: modeStr = "IRQ"; break;
		case 3: modeStr = "SVC"; break;
		case 15: modeStr = "Kernel"; break;
	}
	iprintf("\n\nCPU mode: %s\n", modeStr);
	iprintf("\nA: view stacktrace if available\n");
}

typedef int (*bt_func)(void* address, int depth /* 0-based */, void* user_data);
typedef bool (*btDumpFunc)(word_t* pCtx, bt_func pFunc, void* user_data);
static int stCallback(void* address, int depth /* 0-based */, void* user_data);

static void showStacktrace()
{
	iprintf("\x1b[2J\x1b[5CStacktrace\n\n");
	instance_t cxxInst = FeOS_GetModule("feoscxx");
	if (!cxxInst)
		iprintf("Unavailable: feoscxx not loaded\n\n");
	else
	{
		btDumpFunc btDump = (btDumpFunc) FeOS_FindSymbol(cxxInst, "FeOS_BacktraceDump");
		btDump((word_t*)exceptionRegisters, stCallback, NULL);
		iprintf("\n");
	}
	iprintf("A: view register meanings");
}

int stCallback(void* address, int depth /* 0-based */, void* user_data)
{
	part_t oPart;

	if (depth == 19) return 0; // prevent screen overflow

	if (resolveAddr(&oPart, address) && *oPart.name != '(')
		iprintf("%d - %s+0x%X\n", depth, oPart.name, oPart.offset);
	else
		iprintf("%d - 0x%08X\n", depth, (word_t)address);
	return 1;
}

static inline void crappyWaitForVBlank()
{
	while (REG_VCOUNT == 192);
	while (REG_VCOUNT != 192);
}

static inline word_t crappyKeysHeld()
{
	return (~REG_KEYINPUT) & 0x3ff;
}

// The following is based on libnds code

#define R(x) exceptionRegisters[x]

static word_t armOp2(word_t val, word_t shift)
{
	if (shift == 0x0B) return val; // no shift
	int index;
	if (shift & 1) // shift by a register
		index = R((shift >> 4) & 0xF);
	else // shift by a constant
		index = (shift >> 3) & 0x1F;

	switch (shift & 6)
	{
		case 0: return val << index;      // LSL
		case 2: return val >> index;      // LSR
		case 4: return (int)val >> index; // ASR
		case 6: // ROR
			index &= 0x1F;
			return (val >> index) | (val << (32-index));
	}
	return val;
}

static word_t resolveErrorAddress_ARM(word_t opcode)
{
	if ((opcode & 0x0FB00FF0) == 0x01000090) // swp: xxxx0001 0x00nnnn dddd0000 1001mmmm
		return R((opcode >> 16) & 0xF);
	if ((opcode & 0x0C000000) == 0x04000000) // str/ldr: xxxx01xx xxxxnnnn ddddffff ffffffff
	{
		word_t base = R((opcode >> 16) & 0xF);
		if (!(opcode & 0x01000000)) return base; // post-indexing
		// Pre-indexing
		int offset = 0;
		if (opcode & 0x02000000) // Register offset
			offset = (int) armOp2(R(opcode & 0xF), (opcode >> 4) & 0xFF);
		else // Immediate offset
			offset = opcode & 0xFFF;
		return base + ((opcode & 0x00800000) ? offset : -offset);
	}
	if ((opcode & 0x0E400F90) == 0x00000090)
	{
		// ldrh/strh w/ register Rm: xxxx000x x0xxnnnn dddd0000 1xx1mmmm
		int offset = (int) armOp2(R(opcode & 0xF), (opcode >> 4) & 0xFF);
		return R((opcode >> 16) & 0xF) + ((opcode & 0x00800000) ? offset : -offset);
	}
	if ((opcode & 0x0E400F90) == 0x00400090)
	{
		// ldrh/strh w/ imm offset: xxxx000x x1xxnnnn ddddffff 1xx1ffff
		int offset = (opcode & 0xF) | ((opcode & 0xF00) >> 8);
		return R((opcode >> 16) & 0xF) + ((opcode & 0x00800000) ? offset : -offset);
	}
	if ((opcode & 0x0E000000) == 0x08000000) // ldm/stm: xxxx100x xxxxnnnn llllllll llllllll
		return R((opcode >> 16) & 0xF);
	return 0;
}

static word_t resolveErrorAddress_Thumb(word_t opcode)
{
	if ((opcode & 0xF800) == 0x4800) // ldr r,[pc,###]   01001ddd ffffffff
		return R(15) + (s8)(opcode & 0xff);
	if ((opcode & 0xF200) == 0x5000) // ldr r,[r,r]      0101xx0f ffbbbddd
		return R((opcode >> 3) & 7) + R((opcode >> 6) & 7);
	if ((opcode & 0xF200) == 0x5200) // ldrsh            0101xx1f ffbbbddd
		return R((opcode >> 3) & 7) + R((opcode >> 6) & 3);
	if ((opcode & 0xE000) == 0x6000 || (opcode & 0xF000) == 0x8000)
		// ldr r,[r,imm] 011xxfff ffbbbddd
		// ldrh          1000xfff ffbbbddd
		return R((opcode >> 3) & 7) + (((opcode >> 6) & 0x1F) << 2);
	if ((opcode & 0xF000) == 0x9000) // ldr r,[sp,###]   1001xddd ffffffff
		return R(13) + (s8)(opcode & 0xff);
	if ((opcode & 0xF700) == 0xB500) // push/pop         1011x10l llllllll
		return R(13);
	if ((opcode & 0xF000) == 0xC000) // ldm/stm          1100xbbb llllllll
		return R((opcode >> 8) & 7);
	return 0;
}

#undef R

static word_t resolveErrorAddress(void* pOpcode, bool isThumb)
{
	if (isThumb)
		return resolveErrorAddress_Thumb(*(hword_t*)pOpcode);
	else
		return resolveErrorAddress_ARM(*(word_t*)pOpcode);
}

void ExcptHandler_C()
{
	REG_IME = 0;
	videoReset();
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);
	PrintConsole* conmain = consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 0, 1, true, true);
	memcpy(&oConSub, consoleGetDefault(), sizeof(PrintConsole));
	PrintConsole* consub = consoleInit(&oConSub, 0, BgType_Text4bpp, BgSize_T_256x256, 0, 1, false, true);
	InstallConThunks();

	consoleSelect(conmain);

	BG_PALETTE[0] = RGB15(31,0,31);
	BG_PALETTE[255] = RGB15(31,31,31);
	BG_PALETTE_SUB[0] = RGB15(31,0,31);
	BG_PALETTE_SUB[255] = RGB15(31,31,31);

	iprintf("\x1b[5CGuru Meditation Error!\n");
	word_t currentMode = getCPSR() & 0x1f;
	word_t codeAddress, exceptionAddress = 0;
	bool thumbState = !!((*(u32*)0x02FFFD90) & 0x20);
	int offset = 8;

	if (currentMode == 0x17)
	{
		iprintf("\x1b[10Cdata abort!\n\n");
		codeAddress = exceptionRegisters[15] - offset;
		if ((codeAddress > 0x02000000 && codeAddress < 0x03000000) ||
		    (codeAddress > (word_t)__itcm_start && codeAddress < (word_t)(__itcm_start + 0x8000)))
			exceptionAddress = resolveErrorAddress((void*)codeAddress, thumbState);
		else
			exceptionAddress = codeAddress;
	} else
	{
		offset = thumbState ? 2 : 4;
		iprintf("\x1b[5Cundefined instruction!\n\n");
		codeAddress = exceptionRegisters[15] - offset;
		exceptionAddress = codeAddress;
	}

	iprintf("  pc: %08X addr: %08X\n\n", codeAddress, exceptionAddress);

	int i;
	for (i = 0; i < 8; i ++)
		iprintf("  %s: %08X   %s: %08X\n",
			registerNames[i],   exceptionRegisters[i],
			registerNames[i+8], exceptionRegisters[i+8]);
	iprintf("\n");

	word_t* stack = (word_t*) exceptionRegisters[13];
	for (i = 0; i < 10; i ++)
		iprintf("\x1b[%d;2H%08X:  %08X %08X", i+14, (word_t)&stack[i*2], stack[i*2], stack[(i*2)+1]);

	consoleSelect(consub);

	exceptionRegisters[15] = (word_t)codeAddress; // make it pretty

	showRegMeanings();

	word_t keysOld = 0, keys = crappyKeysHeld();
	int mode = 0;

	for(;;)
	{
		crappyWaitForVBlank();
		keysOld = keys;
		keys = crappyKeysHeld();

		word_t kDown = keys &~ keysOld;
		if (kDown & KEY_A)
		{
			mode ++;
			if (mode == 2) mode = 0;
			switch (mode)
			{
				case 0:
					showRegMeanings();
					break;
				case 1:
					showStacktrace();
					break;
			}
		}
	}
}
