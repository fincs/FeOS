#include "common.h"

static int hwMain(void* userData)
{
	// Place your video inits here

	for (;;)
	{
		swiWaitForVBlank();
		scanKeys();

		// Place your VBlank code here
		word_t kDown = keysDown();
		if (kDown & KEY_START)
			break;
	}

	// Place any necessary video deinits here

	return 0;
}

int main()
{
	// Request temporal ownership of the DS video hardware in
	// order to execute hwMain(). If the request is denied,
	// the function will not be executed and -1 will be returned.
	return DSRequestHardware(hwMain, NULL, NULL);
}
