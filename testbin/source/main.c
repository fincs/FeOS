#include <feos.h>
#include <stdio.h>
#include <string.h>

int i;

FEOSINIT void foobar_init()
{
	FeOS_DebugPrint("[INIT]\n");
	i = 42;
}

FEOSFINI void foobar_fini()
{
	FeOS_DebugPrint("[FINI]\n");
}

int getNumber();
void TestCpp();
void ImpFunc();

int main(int argc, const char** argv)
{
	int j;

	FeOS_DebugPrint("FeOS test executable\n");
	printf("argc: %d\n", argc);
	for(j = 0; j < argc; j ++)
		printf("[%d] %s\n", j, argv[j]);

	for(j = 0; j < 60; j ++)
		FeOS_WaitForVBlank();

	FeOS_DebugPrint("\nVBlank test OK\n\n");

	TestCpp();

	/*
	FeOS_DebugPrint("\nTesting exceptions...\n");
	for(j = 0; j < 60; j ++)
		FeOS_WaitForVBlank();
	*(volatile word_t*) 0x04000208 = 0; // REG_IME
	FeOS_DebugPrint("Exception not caught!\n");

	for(;;)
		FeOS_WaitForVBlank();
	*/

	FeOS_DebugPrint("\n");
	return 0;
}
