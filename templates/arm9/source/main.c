/*---------------------------------------------------------------------------------

	$Id: main.c,v 1.5 2007-10-23 00:46:29 wntrmute Exp $

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
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);	//sub bg 0 will be used to print text
	vramSetBankC(VRAM_C_SUB_BG);

	SUB_BG0_CR = BG_MAP_BASE(31);

	BG_PALETTE_SUB[255] = RGB15(31,31,31);	//by default font will be rendered with color 255

	//consoleInit() is a lot more flexible but this gets you up and running quick
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);

	iprintf("\n\n\tHello DS dev'rs\n");
	iprintf("\twww.drunkencoders.com");

	while(1) {

		touchXY=touchReadXY();
		iprintf("\x1b[10;0HTouch x = %04X, %04X\n", touchXY.x, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.y, touchXY.py);

		swiWaitForVBlank();
	}

	return 0;
}
