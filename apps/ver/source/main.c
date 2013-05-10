#include <stdio.h>
#include <feos.h>

int main()
{
	const systeminfo_t* info = FeOS_GetSystemInfo();
	printf(
		"FeOS%s [Version %s]\n"
		"\n"
		"System: %s\n"
		"%s\n"
		, (info->systemFlags & SYSTEM_FLAGS_DEBUG) ? " Debug" : ""
		, info->osVersionText, info->systemTypeText, info->systemModeText);
	if (info->systemFlags & SYSTEM_FLAGS_EMULATOR)
		printf("Running on a DS emulator\n");
	return 0;
}
