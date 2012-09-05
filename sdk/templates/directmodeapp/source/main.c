#include "common.h"

int main()
{
	FeOS_DirectMode();

	// Place your video inits here

	for (;;)
	{
		swiWaitForVBlank();
		// Place your VBlank code here
		word_t kDown = keysDown();
		if (kDown & KEY_START)
			break;
	}

	FeOS_ConsoleMode();
	return 0;
}
