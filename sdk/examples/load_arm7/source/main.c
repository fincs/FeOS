#include <stdio.h>
#include <feos.h>

int main()
{
	printf("ARM7 load test\n");
	int fifoCh;
	module_t hArm7Mod = DSLoadARM7("/data/FeOS/arm7/arm7let.fx2", &fifoCh);
	printf("Module handle: %p\nFIFO channel: %d\n", hArm7Mod, fifoCh);
	if (hArm7Mod)
	{
		int i;
		for (i = 0; i < 60; i ++) swiWaitForVBlank();
		DSFreeARM7(hArm7Mod, fifoCh);
	}
	return 0;
}
