/*---------------------------------------------------------------------------------

	derived from the default ARM7 core

		Copyright (C) 2005 - 2010
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
#include <maxmod7.h>
#include <nds/bios.h>

//---------------------------------------------------------------------------------
void VblankHandler(void) {
//---------------------------------------------------------------------------------
	Wifi_Update();
}


//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	inputGetAndSend();
}

volatile bool exitflag = false;

//---------------------------------------------------------------------------------
void i2cIRQHandler() {
//---------------------------------------------------------------------------------
	int cause = (i2cReadRegister(I2C_PM, 0x10) & 0x3) | (i2cReadRegister(I2C_GPIO, 0x02)<<2);
	
	if (cause & 1) exitflag = true;
}

//---------------------------------------------------------------------------------
int main() {
//---------------------------------------------------------------------------------
	readUserSettings();

	irqInit();
	fifoInit();

	mmInstall(FIFO_MAXMOD);
	// Start the RTC tracking IRQ
	initClockIRQ();

	SetYtrigger(80);

	installWifiFIFO();
	installSoundFIFO();

	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);
	irqSetAUX(IRQ_I2C, i2cIRQHandler);
	irqEnableAUX(IRQ_I2C);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);   

	// Keep the ARM7 mostly idle
	while (!exitflag) {
		if ( 0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
			exitflag = true;
		}

		
		if(fifoCheckValue32(FIFO_USER_01))
		{
			//we don't care what value got sent. all we're interested in is the signal
			fifoGetValue32(FIFO_USER_01);

			int t1,t2;
			u32 temp=touchReadTemperature(&t1, &t2);
			//these will queue up
			fifoSendValue32(FIFO_USER_01,t1);
			fifoSendValue32(FIFO_USER_01,t2);
			fifoSendValue32(FIFO_USER_01,temp);
		}

		//ideally we would wait for irq here to keep from burning cpu.
		//but arm7 bios routines seem not to be readily available at this point.
	}
	return 0;
}
