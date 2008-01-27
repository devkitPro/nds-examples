/*---------------------------------------------------------------------------------
	$Id: template.c,v 1.5 2008-01-27 18:40:49 dovoto Exp $

	Basic Hello World

	


---------------------------------------------------------------------------------*/
#include "nds.h"
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	touchPosition touchXY;

	videoSetMode(0);	//not using the main screen
	consoleDemoInit();  //setup the sub screen for basic printing

	iprintf("\n\n\tHello World!\n");

	while(1) {

		touchXY=touchReadXY();
		iprintf("\x1b[10;0HTouch x = %04X, %04X\n", touchXY.x, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.y, touchXY.py);

	}

	return 0;
}
