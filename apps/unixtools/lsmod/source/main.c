#include <stdio.h>
#include <feos.h>

static int ModuleEnumCb(instance_t hInst, const char* name, word_t size, void* user_data)
{
	if (hInst != FeOS_GetInstance())
		printf("%08X %s: %d bytes\n", (word_t)hInst, name, size);
	return ENUM_CONTINUE;
}

int main()
{
	printf("Address, Module name and Size\n\n");
	FeOS_EnumModules(ModuleEnumCb, NULL);
	return 0;
}
