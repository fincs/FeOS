#include <feos.h>
#include <stdio.h>

#ifndef REG_VCOUNT
#define REG_VCOUNT (*(volatile hword_t*)0x4000006)
#endif

volatile int count = 0;

void hblank()
{
	int ff = (REG_VCOUNT + count) & 0x1F;
	BG_PALETTE[0] = RGB15(ff, 0, 0);
	BG_PALETTE_SUB[0] = RGB15(0, ff, 0);
}

int main()
{
	printf("Direct Mode Demo\n");
	printf("Press A to continue\n");
	for (;;)
	{
		swiWaitForVBlank();
		if (keysDown() & KEY_A)
			break;
	}

	DSDirectMode();

	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	irqSet(IRQ_HBLANK, hblank);
	irqEnable(IRQ_HBLANK);

	for (;;)
	{
		swiWaitForVBlank();
		count ++;
		if (keysDown() & KEY_A)
			break;
	}

	irqDisable(IRQ_HBLANK);
	irqSet(IRQ_HBLANK, NULL);

	DSConsoleMode();
	printf("End of direct mode demo\n");

	return 0;
}
