#include "feos.h"
#include <stdio.h>

void FeOS_IRQPoll();

void FeOS_ProcessIRQ(word_t flags)
{
	// TODO: write actual user-mode IRQ handler
	//if (flags & IRQ_VBLANK)
	//{
	//	iprintf("{User-mode VBL handler}");
	//}
}

void FeOS_WaitForIRQ(word_t mask)
{
	for(;;)
	{
		INTR_WAIT_FLAGS = 0;
		FeOS_IRQPoll();
		word_t flags = INTR_WAIT_FLAGS;
		FeOS_ProcessIRQ(flags);
		if (flags & mask)
			break;
	}
}
