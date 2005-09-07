/*---------------------------------------------------------------------------------
	$Id: template.c,v 1.2 2005-09-07 20:06:06 wntrmute Exp $

	Simple ARM7 stub (sends RTC, TSC, and X/Y data to the ARM 9)

	$Log: not supported by cvs2svn $
	Revision 1.8  2005/08/03 05:13:16  wntrmute
	corrected sound code


---------------------------------------------------------------------------------*/
#include <nds.h>

#include <nds/bios.h>
#include <nds/arm7/touch.h>
#include <nds/arm7/clock.h>

//---------------------------------------------------------------------------------
void startSound(int sampleRate, const void* data, u32 bytes, u8 channel, u8 vol,  u8 pan, u8 format) {
//---------------------------------------------------------------------------------
	SCHANNEL_TIMER(channel)  = SOUND_FREQ(sampleRate);
	SCHANNEL_SOURCE(channel) = (u32)data;
	SCHANNEL_LENGTH(channel) = bytes >> 2 ;
	SCHANNEL_CR(channel)     = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(vol) | SOUND_PAN(pan) | (format==1?SOUND_8BIT:SOUND_16BIT);
}


//---------------------------------------------------------------------------------
s32 getFreeSoundChannel() {
//---------------------------------------------------------------------------------
	int i;
	for (i=0; i<16; i++) {
		if ( (SCHANNEL_CR(i) & SCHANNEL_ENABLE) == 0 ) return i;
	}
	return -1;
}

//---------------------------------------------------------------------------------
void VblankHandler(void) {
//---------------------------------------------------------------------------------
	static int heartbeat = 0;

	uint16 but=0, x=0, y=0, xpx=0, ypx=0, z1=0, z2=0, batt=0, aux=0;
	int t1=0, t2=0;
	uint32 temp=0;
	uint8 ct[sizeof(IPC->curtime)];
	u32 i;

	// Update the heartbeat
	heartbeat++;

	// Read the touch screen

	but = REG_KEYXY;

	if (!(but & (1<<6))) {

		touchPosition tempPos = touchReadXY();

		x = tempPos.x;
		y = tempPos.y;
		xpx = tempPos.px;
		ypx = tempPos.py;
	}

	z1 = touchRead(TSC_MEASURE_Z1);
	z2 = touchRead(TSC_MEASURE_Z2);

	
	batt = touchRead(TSC_MEASURE_BATTERY);
	aux  = touchRead(TSC_MEASURE_AUX);

	// Read the time
	rtcGetTime((uint8 *)ct);
	BCDToInteger((uint8 *)&(ct[1]), 7);

	// Read the temperature
	temp = touchReadTemperature(&t1, &t2);

	// Update the IPC struct
	IPC->heartbeat	= heartbeat;
	IPC->buttons		= but;
	IPC->touchX			= x;
	IPC->touchY			= y;
	IPC->touchXpx		= xpx;
	IPC->touchYpx		= ypx;
	IPC->touchZ1		= z1;
	IPC->touchZ2		= z2;
	IPC->battery		= batt;
	IPC->aux			= aux;

	for(i=0; i<sizeof(ct); i++) {
		IPC->curtime[i] = ct[i];
	}

	IPC->temperature = temp;
	IPC->tdiode1 = t1;
	IPC->tdiode2 = t2;


	//sound code  :)
	TransferSound *snd = IPC->soundData;
	IPC->soundData = 0;

	if (0 != snd) {

		for (i=0; i<snd->count; i++) {
			s32 chan = getFreeSoundChannel();

			if (chan >= 0) {
				startSound(snd->data[i].rate, snd->data[i].data, snd->data[i].len, chan, snd->data[i].vol, snd->data[i].pan, snd->data[i].format);
			}
		}
	}

}

//---------------------------------------------------------------------------------
int main(int argc, char ** argv) {
//---------------------------------------------------------------------------------

	// Reset the clock if needed
	rtcReset();

	//enable sound
	powerON(POWER_SOUND);
	SOUND_CR = SOUND_ENABLE | SOUND_VOL(0x7F);
	IPC->soundData = 0;

	irqInit();
	irqSet(IRQ_VBLANK, VblankHandler);
	irqEnable(IRQ_VBLANK);

	// Keep the ARM7 out of main RAM
	while (1) swiWaitForVBlank();
}


