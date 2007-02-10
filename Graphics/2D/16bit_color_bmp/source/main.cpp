#include <nds.h>
#include <stdio.h>

// git outputs a nice header to reference data
#include "drunkenlogo.h"

int getSize(uint8 *source, uint16 *dest, uint32 arg) {
	return *(uint32*)source;
}

uint8 readByte(uint8 *source) {
 	return *source;
}
TDecompressionStream drunkenlogo_decomp = {
  getSize,
  NULL,
  readByte
};

int main(void) {
	// irqs are nice
	irqInit();
	irqEnable(IRQ_VBLANK);

    // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	// set the sub background up for text display (we could just print to one
	// of the main display text backgrounds just as easily
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE); //sub bg 0 will be used to print text

    // set the first bank as background memory and the third as sub background memory
    // B and D are not used (if you want a bitmap greater than 256x256 you will need more
    // memory so another vram bank must be used and mapped consecutivly
    vramSetMainBanks(	VRAM_A_MAIN_BG_0x06000000, VRAM_B_LCD,
						VRAM_C_SUB_BG , VRAM_D_LCD);

	// set up text background for text
    SUB_BG0_CR = BG_MAP_BASE(31);

	BG_PALETTE_SUB[255] = RGB15(31,31,31);//by default font will be rendered with color 255

	//consoleInit() is a lot more flexible but this gets you up and running quick
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\t16 bit bitmap demo");

	// set up our bitmap background

	BG3_CR = BG_BMP16_256x256;

	// these are rotation backgrounds so you must set the rotation attributes:
    // these are fixed point numbers with the low 8 bits the fractional part
    // this basicaly gives it a 1:1 translation in x and y so you get a nice flat bitmap
	BG3_XDX = 1 << 8;
	BG3_XDY = 0;
	BG3_YDX = 0;
	BG3_YDY = 1 << 8;

	BG3_CX = 0;
	BG3_CY = 0;

	swiDecompressLZSSVram((void*)drunkenlogoBitmap, BG_GFX, 0, &drunkenlogo_decomp);
	while(1) swiWaitForVBlank();

	return 0;
}
