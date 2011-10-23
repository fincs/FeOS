#include <feos.h>
#include <stdio.h>

#define REG_DISPCNT     (*(volatile word_t*)0x04000000)
#define REG_DISPCNT_SUB (*(volatile word_t*)0x04001000)
#define MODE_0_2D       0x10000
#define BG_PALETTE      ((volatile hword_t*)0x05000000)
#define BG_PALETTE_SUB  ((volatile hword_t*)0x05000400)
#define REG_VCOUNT      (*(volatile hword_t*)0x4000006)
#define RGB15(r,g,b)    ((r)|((g)<<5)|((b)<<10))

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

	REG_DISPCNT     = MODE_0_2D;
	REG_DISPCNT_SUB = MODE_0_2D;

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
