#include <feos.h>
#include <stdio.h>
#include <stdlib.h>
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

void TestCpp();

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

	FeOS_DebugPrint("\n");
	return 0;
}
