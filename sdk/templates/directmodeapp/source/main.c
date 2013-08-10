#include "common.h"

int main()
{
	DSDirectMode();

	// Place your video inits here

	for (;;)
	{
		swiWaitForVBlank();
		// Place your VBlank code here
		word_t kDown = keysDown();
		if (kDown & KEY_START)
			break;
	}

	DSConsoleMode();
	return 0;
}
