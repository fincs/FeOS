#include "feos.h"
#include <sys/iosupport.h>

// stdio FeOS-compatible keyboard read function
// This function is a hack of that of libnds

#define KEY_BUFFER_SIZE 256

// At least libnds does not define these as static...
extern int keyBufferOffset;
extern int keyBufferLength;
extern int lastKey;
extern bool stdioRead;
extern char keyBuffer[KEY_BUFFER_SIZE];
extern Keyboard* curKeyboard;

int FeOS_swi_keyboardUpdate();

ssize_t FeOS_KeybdRead(struct _reent *r, int unused, char *ptr, size_t len)
{
	//int wasHidden = 0;
	int tempLen;
	int c = NOKEY;

	stdioRead = true;

	// FeOS: this never happens
	/*
	if (!curKeyboard->visible)
	{
		wasHidden = 1;
		keyboardShow();
	}
	*/

	do
	{
		// FeOS: wait for VBlank.
		//   - It avoids unnecessarily killing the CPU
		//   - It allows user IRQs to run
		FeOS_WaitForVBlank();
		FeOS_swi_keyboardUpdate();
	} while(keyBufferLength <= 0 || (keyBufferLength < KEY_BUFFER_SIZE && lastKey != DVK_ENTER));

	tempLen = keyBufferLength;

	while (len > 0 && keyBufferLength > 0)
	{
		c = keyBuffer[(keyBufferOffset - keyBufferLength) % 256];

		*ptr++ = c;

		keyBufferLength--;
		len--;
	}

	// FeOS: this never happens
	/*
	if (wasHidden)
		keyboardHide();
	*/

	stdioRead = false;

	return tempLen;
}
