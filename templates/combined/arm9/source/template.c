/*---------------------------------------------------------------------------------

	Basic Hello World

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	touchPosition touchXY;

	irqInit();
	irqEnable(IRQ_VBLANK);

	videoSetMode(0);	//not using the main screen
	consoleDemoInit();  //setup the sub screen for basic printing

	iprintf("\n\n\tHello World!\n");

	while(1) {

		touchXY=touchReadXY();
		iprintf("\x1b[10;0HTouch x = %04X, %04X\n", touchXY.x, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.y, touchXY.py);

		swiWaitForVBlank();
	}

	return 0;
}
