#include <stdio.h>
#include <feos.h>

fp_t oldFunc = NULL;

void vbl()
{
	static int i = 0;
	printf("VBlank %d\n", i ++);
	if (oldFunc) oldFunc();
}

int main()
{
	oldFunc = irqSet(IRQ_VBLANK, vbl);
	for (;;)
	{
		swiWaitForVBlank();
		if (keysDown() & KEY_START)
			break;
	}
	irqSet(IRQ_VBLANK, oldFunc);
	return 0;
}
