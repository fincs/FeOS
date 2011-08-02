#include <feos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int i;

FEOSINIT void foobar_init()
{
	printf("[INIT]\n");
	i = 42;
}

FEOSFINI void foobar_fini()
{
	printf("[FINI]\n");
}

void TestCpp();

ssize_t MyWrite(void* cData, const char* buf, size_t size)
{
	return fwrite(buf, 1, size, stdout);
}

const stream_t MyStream = { 0, NULL, NULL, MyWrite, NULL, NULL };

void StreamTest()
{
	FILE* stm = FeOS_OpenStream(&MyStream);
	fputs("\nStreams do work!\n", stm);
	fclose(stm);
}

int main(int argc, const char** argv)
{
	int j;

	printf("FeOS test executable\n");
	printf("argc: %d\n", argc);
	for(j = 0; j < argc; j ++)
		printf("[%d] %s\n", j, argv[j]);

	StreamTest();

	for(j = 0; j < 60; j ++)
		FeOS_WaitForVBlank();

	printf("\nVBlank test OK\n\n");

	TestCpp();

	printf("\n");

	printf("Running hello_world...\n");

	const char* hwlaunchargs[] = { "hello_world" };
	int rc = FeOS_Execute(1, hwlaunchargs);

	if (rc != E_FILENOTFOUND)
		printf("Return code: %d\n", rc);
	else
		printf("hello_world does not exist...\n");

	printf("\n");

	return 0;
}
