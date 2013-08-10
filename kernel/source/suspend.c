#include "feos.h"
#include "feosfifo.h"

static int suspendMode = MODE_NORMAL;

int DSSetSuspendMode(int mode)
{
	if (mode == MODE_GET) return suspendMode;
	sassert(mode >= 0 && mode < MODE_COUNT, ERRSTR_INVALIDPARAM);

	int oldMode = suspendMode;
	suspendMode = mode;
	return oldMode;
}

static volatile u16 oldPowerCnt;
volatile bool inHeadphoneSleep = false;

static void altSystemSleep()
{
	oldPowerCnt = REG_POWERCNT;
	REG_POWERCNT = oldPowerCnt &~ BIT(0); // turn LCD off

	FeOSFifoMsg msg;
	msg.type = FEOS_ARM7_HEADPHONE_SLEEP;
	fifoSendDatamsg(FIFO_FEOS, sizeof(msg), (void*)&msg);
	inHeadphoneSleep = true;
}

static void FeOSAddrHandler(void* addr, void* user_data)
{
	switch ((int)addr)
	{
		case (int)FEOS_ARM9_WAKEUP_ADDR:
		{
			REG_POWERCNT = oldPowerCnt;
			inHeadphoneSleep = false;
			break;
		}
	}
}

void __real_systemValueHandler(u32 value, void* data);
void __wrap_systemValueHandler(u32 value, void* data)
{
	if (value != PM_REQ_SLEEP)
	{
		__real_systemValueHandler(value, data);
		return;
	}

	switch (suspendMode)
	{
		case MODE_DISABLED:                     return;
		case MODE_NORMAL:     systemSleep();    return;
		case MODE_HEADPHONES: altSystemSleep(); return;
		default:              sassert(false, "Invalid suspend mode");
	}
}

void installFeOSFIFO()
{
	fifoSetAddressHandler(FIFO_FEOS, FeOSAddrHandler, NULL);
}
