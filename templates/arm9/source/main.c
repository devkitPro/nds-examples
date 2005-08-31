/*---------------------------------------------------------------------------------

	$Id: main.c,v 1.2 2005-08-31 01:26:30 wntrmute Exp $

	Simple console print demo
	-- dovoto

	$Log: not supported by cvs2svn $
	Revision 1.1  2005/08/03 06:29:56  wntrmute
	added templates


---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <nds/arm9/console.h> //basic print funcionality

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

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

		//move the cursor
		printAt(0,10);
		iprintf("Touch x = %04X, %04X\n", IPC->touchX, IPC->touchXpx);
		iprintf("Touch y = %04X, %04X\n", IPC->touchY, IPC->touchYpx);

	}

	return 0;
}
