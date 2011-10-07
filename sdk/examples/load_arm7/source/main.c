#include <stdio.h>
#include <feos.h>

int main()
{
	printf("ARM7 load test\n");
	int fifoCh;
	instance_t arm7_inst = FeOS_LoadARM7("/data/FeOS/arm7/arm7let.fx2", &fifoCh);
	printf("inst %08X\nfifo ch %d\n", (word_t)arm7_inst, fifoCh);
	if (arm7_inst)
	{
		int i;
		for (i = 0; i < 60; i ++) FeOS_WaitForVBlank();
		FeOS_FreeARM7(arm7_inst, fifoCh);
	}
	return 0;
}
