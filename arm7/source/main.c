/*---------------------------------------------------------------------------------

	FeOS ARM7 binary
	Based on libnds' default ARM7 core, which is:

		Copyright (c) 2005 - 2011
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.

	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.

	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/

#include <nds.h>
#include <dswifi7.h>
#include "feos.h"

void VCountHandler()
{
	inputGetAndSend();
}

static volatile bool exitflag = false;

void powerButtonCB()
{
	exitflag = true;
}

int main()
{
	readUserSettings();
	ledBlink(0);

	irqInit();
	initClockIRQ();
	fifoInit();

	SetYtrigger(80);

	installWifiFIFO();
	//installSoundFIFO();
	installSystemFIFO();
	installFeOSFIFO();

	irqSet(IRQ_VCOUNT, VCountHandler);

	irqEnable(IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

	setPowerButtonCB(powerButtonCB);

	while (!exitflag)
	{
		swiWaitForVBlank();
		FeOS_VBlankFunc();
		if (!(REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R)))
			break;
		Wifi_Update();
	}
	return 0;
}
