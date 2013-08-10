#include <stdio.h>
#include <feos.h>

static int ModuleEnumCb(module_t hMod, const char* name, word_t size, void* user_data)
{
	if (hMod != LdrGetSelf())
		printf("%08X %s: %d bytes\n", (word_t)hMod, name, size);
	return ENUM_CONTINUE;
}

int main()
{
	printf("Address, Module name and Size\n\n");
	LdrEnumModules(ModuleEnumCb, NULL);
	return 0;
}
