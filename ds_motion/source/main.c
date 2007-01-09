/*---------------------------------------------------------------------------------
	$Id: main.c,v 1.1 2007-01-09 06:14:40 dovoto Exp $

	Basic Hello World

	$Log: not supported by cvs2svn $
	Revision 1.4  2005/09/17 23:15:13  wntrmute
	corrected iprintAt in templates
	
	Revision 1.3  2005/09/05 00:32:20  wntrmute
	removed references to IPC struct
	replaced with API functions
	
	Revision 1.2  2005/08/31 01:24:21  wntrmute
	updated for new stdio support

	Revision 1.1  2005/08/03 06:29:56  wntrmute
	added templates


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
