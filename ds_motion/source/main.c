/*---------------------------------------------------------------------------------
	$Id: main.c,v 1.6 2008-11-29 22:59:00 wntrmute Exp $

	Basic DS Motion example

	$Log: not supported by cvs2svn $
	Revision 1.5  2007/05/22 18:04:16  dovoto
	Updated for new ds motion code.  motion_enable is now motion_init
	
	Revision 1.4  2007/01/10 16:57:30  dovoto
	Fixed a small bug with callibration code that prevented it from showing message
	
	Revision 1.3  2007/01/10 16:43:18  dovoto
	Update motion example to demonstrate new calibration functionality
	
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
	
	consoleDemoInit();
	
	motion_init();

	while(1) 
	{	
		scanKeys();
		
		if(keysDown() & KEY_A) Calibrate();
		
		consoleClear();
		iprintf("Nds is%s inserted\n", motion_init() ? "" : " not");
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
	
	scanKeys();
	
	consoleClear();
	
	iprintf("Set the DS on a flat table...\nPress A\n");
	
	while(!keysDown() & KEY_A) scanKeys();
	
	//these set the zero points of the accelerometers and gryo
	motion_set_offs_x();
	motion_set_offs_y();
	motion_set_offs_gyro();
	
	//this should be set to the acceleration value at 1 z
	motion_set_sens_z(motion_read_z());
}
