/*---------------------------------------------------------------------------------

	$Id: main.c,v 1.2 2006-06-19 19:10:03 wntrmute Exp $

	demo of ansi escape sequences

	$Log: not supported by cvs2svn $
	Revision 1.1  2005/09/13 15:01:07  wntrmute
	added ansi escape sequences demo


---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	// initialise the irq dispatcher
	irqInit();
	// a vblank interrupt is needed to use swiWaitForVBlank()
	// since the dispatcher handles the flags no handler is required
	irqEnable(IRQ_VBLANK);
	videoSetMode(0);	//not using the main screen
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);	//sub bg 0 will be used to print text
	vramSetBankC(VRAM_C_SUB_BG);

	SUB_BG0_CR = BG_MAP_BASE(31);

	BG_PALETTE_SUB[255] = RGB15(31,31,31);	//by default font will be rendered with color 255

	//consoleInit() is a lot more flexible but this gets you up and running quick
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);

	// ansi escape sequence to clear screen and home cursor
	// /x1b[line;columnH
	iprintf("\x1b[2J");

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("\x1b[10;10HHello World!");

	// ansi escape sequence to move cursor up
	// /x1b[linesA
	iprintf("\x1b[10ALine 0");

	// ansi escape sequence to move cursor left
	// /x1b[columnsD
	iprintf("\x1b[28DColumn 0");

	// ansi escape sequence to move cursor down
	// /x1b[linesB
	iprintf("\x1b[19BLine 19");

	// ansi escape sequence to move cursor right
	// /x1b[columnsC
	iprintf("\x1b[5CColumn 20");

	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}
