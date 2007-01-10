/*---------------------------------------------------------------------------------
	$Id: main.c,v 1.3 2007-01-10 16:43:18 dovoto Exp $

	Basic DS Motion example

	$Log: not supported by cvs2svn $
	Revision 1.2  2007/01/09 06:23:05  dovoto
	Fixed logging header
	
	Revision 1.1  2007/01/09 06:14:40  dovoto
	Added example for ds motion
	
---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

#include <nds/arm9/ndsmotion.h>

void Calibrate();

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	irqInit();
	irqEnable(IRQ_VBLANK);
	
	consoleDemoInit();
	
	motion_enable();

	
	
	while(1) 
	{	
		scanKeys();
		
		if(keysDown() & KEY_A) Calibrate();
		
		consoleClear();
		iprintf("Nds is%s inserted\n", motion_is_inserted() ? "" : " not");
		iprintf("X: raw %i  miliG %i\n", motion_read_x(), motion_acceleration_x());
		iprintf("Y: raw %i  miliG %i\n", motion_read_y(), motion_acceleration_y());
		iprintf("Z: raw %i  miliG %i\n", motion_read_z(), motion_acceleration_z());
		iprintf("R: raw %i  deg/sec %i\n", motion_read_gyro(), motion_rotation());
		iprintf("Press A to calibrate\n");	
		swiWaitForVBlank();
	}

	return 0;
}

//sets the offset of the x and y and gyro and the 1 G offset of z
//to callivrate other offsets you would have to instruct the user to orient
//the DS in various positions
void Calibrate(){

	iprintf("Set the DS on a flat table and press A\n");
	
	while(!keysDown() & KEY_A) scanKeys();
	
	//these set the zero points of the accelerometers and gryo
	motion_set_offs_x();
	motion_set_offs_y();
	motion_set_offs_gyro();
	
	//this should be set to the acceleration value at 1 z
	motion_set_sens_z(motion_read_z());
}
