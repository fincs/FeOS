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

ssize_t DSKeybdRead(struct _reent *r, int unused, char *ptr, size_t len)
{
	int tempLen;
	int c = NOKEY;

	stdioRead = true;

	do
	{
		// FeOS: wait for VBlank.
		//   - It avoids unnecessarily killing the CPU
		//   - It allows user IRQs to run
		DSWaitForVBlank();
		keyboardUpdate();
	} while(keyBufferLength <= 0 || (keyBufferLength < KEY_BUFFER_SIZE && lastKey != DVK_ENTER));

	tempLen = keyBufferLength;

	while (len > 0 && keyBufferLength > 0)
	{
		c = keyBuffer[(keyBufferOffset - keyBufferLength) % 256];

		*ptr++ = c;

		keyBufferLength--;
		len--;
	}

	stdioRead = false;

	return tempLen;
}
