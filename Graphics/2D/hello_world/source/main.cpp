/*---------------------------------------------------------------------------------

	$Id: main.cpp,v 1.7 2006-06-18 21:32:41 wntrmute Exp $

	Simple console print demo
	-- dovoto

	$Log: not supported by cvs2svn $
	Revision 1.6  2005/09/16 12:20:32  wntrmute
	corrected iprintfs
	
	Revision 1.5  2005/09/12 18:32:38  wntrmute
	removed *printAt replaced with ansi escape sequences
	
	Revision 1.4  2005/09/05 00:32:19  wntrmute
	removed references to IPC struct
	replaced with API functions
	
	Revision 1.3  2005/08/31 03:02:39  wntrmute
	updated for new stdio support
	
	Revision 1.2  2005/08/03 06:36:30  wntrmute
	added logging
	added display of pixel co-ords
	

---------------------------------------------------------------------------------*/
#include <nds.h>

#include <stdio.h>

volatile int frame = 0;

//---------------------------------------------------------------------------------
void Vblank() {
//---------------------------------------------------------------------------------
	frame++;
}
	
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touchXY;

	irqInit();
	irqSet(IRQ_VBLANK, Vblank);
	irqEnable(IRQ_VBLANK);
	videoSetMode(0);	//not using the main screen
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);	//sub bg 0 will be used to print text
	vramSetBankC(VRAM_C_SUB_BG); 

	SUB_BG0_CR = BG_MAP_BASE(31);
	
	BG_PALETTE_SUB[255] = RGB15(31,31,31);	//by default font will be rendered with color 255
	
	//consoleInit() is a lot more flexible but this gets you up and running quick
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);

	iprintf("      Hello DS dev'rs\n");
	iprintf("     www.devkitpro.org\n");
	iprintf("   www.drunkencoders.com");

	while(1) {
	
		swiWaitForVBlank();
		touchXY=touchReadXY();

		// print at using ansi escape sequence \x1b[line;columnH 
		iprintf("\x1b[10;0HFrame = %d",frame);
		iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.x, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.y, touchXY.py);		
	
	}

	return 0;
}
