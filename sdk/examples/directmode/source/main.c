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
		FeOS_WaitForVBlank();
		if (FeOS_GetKeysDown() & KEY_A)
			break;
	}

	FeOS_DirectMode();

	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	FeOS_SetInterrupt(IRQ_HBLANK, hblank);
	FeOS_IrqEnable(IRQ_HBLANK);

	for(;;)
	{
		FeOS_WaitForVBlank();
		count ++;
		if (FeOS_GetKeysDown() & KEY_A)
			break;
	}

	FeOS_IrqDisable(IRQ_HBLANK);
	FeOS_SetInterrupt(IRQ_HBLANK, NULL);

	FeOS_ConsoleMode();
	printf("End of direct mode demo\n");

	return 0;
}
