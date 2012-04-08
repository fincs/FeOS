#include "feos.h"
#include "fxe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Replacement for the libnds function

void videoReset();
void InstallConThunks();
void InstallConDummy();

extern PrintConsole oConSub;

// http://user.xmission.com/~emailbox/ascii_cats.htm
static const char* meow[] =
{
	"           ,",
	"          \\)\\_",
	"         /    '. .---._",
	" meow  =P ^     `      '.",
	"        `--.       /     \\",
	"        .-'(       \\      |",
	"       (.-'   )-..__>   , ;",
	"       (_.--``    (__.-/ /",
	"               .-.__.-'.'",
	"         jgs    '-...-'",
	NULL
};

void __attribute__((noreturn)) FeOS_swi_assertfail(const char* file, int line, const char* cond, const char* msg);

void __attribute__((noreturn)) __sassert(const char* file, int line, const char* cond, const char* msg)
{
	if (isUserMode()) FeOS_swi_assertfail(file, line, cond, msg);

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

	iprintf("\x1b[5CKernel assertion failure!\n\nCondition: %s\n\n%s:%d\n\nMessage: %s\n", cond, file, line, msg);

	consoleSelect(consub);

	iprintf("\x1b[5CStacktrace coming soon\n\nInstead, here's a kitten:\n\n");

	const char** i;
	for (i = meow; *i; i ++)
		iprintf("%s\n", *i);

	for (;;)
		swiWaitForVBlank();
}
