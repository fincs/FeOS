#include <nds.h>

int arm7_main(int fifoCh)
{
	powerOn(POWER_SOUND);
	writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE ) | PM_SOUND_AMP );
	REG_SOUNDCNT = SOUND_ENABLE;
	REG_MASTER_VOLUME = 127;

	SCHANNEL_TIMER(8) = SOUND_FREQ(440*8);
	SCHANNEL_CR(8) = SCHANNEL_ENABLE | SOUND_FORMAT_PSG | SOUND_PAN(0x3F) | SOUND_VOL(0x3F);

	return 0;
}

void arm7_fini()
{
	SCHANNEL_CR(8) = 0;
	REG_SOUNDCNT &= ~SOUND_ENABLE;
	writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_AMP ) | PM_SOUND_MUTE );
	// WORKAROUND: The following mustn't be used until the lid closing bug in libnds gets fixed
	//powerOff(POWER_SOUND);
}
