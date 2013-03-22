#include <stdio.h>
#include <feos.h>

void PrintStatus(keys_t keys, const char* verb)
{
	const char* name = NULL;
	     if (keys & KEY_A) name = "A";
	else if (keys & KEY_B) name = "B";
	else if (keys & KEY_X) name = "X";
	else if (keys & KEY_Y) name = "Y";
	else if (keys & KEY_L) name = "L";
	else if (keys & KEY_R) name = "R";
	else if (keys & KEY_UP) name = "Up";
	else if (keys & KEY_DOWN) name = "Down";
	else if (keys & KEY_LEFT) name = "Left";
	else if (keys & KEY_RIGHT) name = "Right";
	else if (keys & KEY_START) name = "Start";
	else if (keys & KEY_SELECT) name = "Select";
	else if (keys & KEY_TOUCH) name = "The stylus";
	if (name) printf("%s was just %sed!\n", name, verb);
}

int main()
{
	printf("Button test (START=exit)\n");
	for (;;)
	{
		swiWaitForVBlank();
		PrintStatus(keysDown(), "press");
		PrintStatus(keysUp(), "releas");

		if(keysHeld() & KEY_TOUCH)
		{
			touchPosition tp;
			touchRead(&tp);
			printf("Stylus: (%d, %d)\n", tp.px, tp.py);
		}

		if(keysDown() & KEY_START) break;
	}
	return 0;
}
