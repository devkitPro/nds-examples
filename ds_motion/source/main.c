/*---------------------------------------------------------------------------------
	$Id: main.c,v 1.2 2007-01-09 06:23:05 dovoto Exp $

	Basic DS Motion example

	$Log: not supported by cvs2svn $
	Revision 1.1  2007/01/09 06:14:40  dovoto
	Added example for ds motion
	
---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

#include <nds/arm9/ndsmotion.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	irqInit();
	irqEnable(IRQ_VBLANK);
	
	consoleDemoInit();
	
	motion_enable();
	
	
	
	while(1) 
	{
			int x = motion_read_x();
			int y = motion_read_y();
			int z = motion_read_z();
			int rot = motion_read_gyro();
			
			consoleClear();
			iprintf("Nds is%s inserted\n", motion_is_inserted() ? "" : " not");
			iprintf("X: raw %i  miliG %i\n", x, motion_acceleration(x));
			iprintf("Y: raw %i  miliG %i\n", y, motion_acceleration(y));
			iprintf("Z: raw %i  miliG %i\n", z, motion_acceleration(z));
			iprintf("R: raw %i  deg/sec %i\n", rot, motion_rotation(rot));
  
			swiWaitForVBlank();
	}

	return 0;
}
