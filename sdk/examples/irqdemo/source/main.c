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
	oldFunc = FeOS_SetInterrupt(IRQ_VBLANK, vbl);
	for(;;)
	{
		FeOS_WaitForVBlank();
		if (FeOS_GetKeysDown() & KEY_START)
			break;
	}
	FeOS_SetInterrupt(IRQ_VBLANK, oldFunc);
	return 0;
}
