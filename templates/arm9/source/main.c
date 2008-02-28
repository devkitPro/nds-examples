/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touchXY;

	irqInit();
	irqEnable(IRQ_VBLANK);

	videoSetMode(0);	//not using the main screen
	consoleDemoInit();  //setup the sub screen for printing

	iprintf("\n\n\tHello DS dev'rs\n");
	iprintf("\twww.drunkencoders.com");

	while(1) {

		touchRead(&touchXY);
		iprintf("\x1b[10;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);

		swiWaitForVBlank();
	}

	return 0;
}
