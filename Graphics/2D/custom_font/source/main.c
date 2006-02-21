#include <nds.h>

#include <nds/arm9/console.h> 
#include <stdio.h>
//Include the font data here Doesn't have to be .h! Can be .c and what not.
#include "font.h"
int main(void) {
//---------------------------------------------------------------------------------

	irqInit();
	irqEnable(IRQ_VBLANK);
	unsigned int i;
	const int char_base = 0;
	const int screen_base = 20;
	videoSetMode(0);	
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);	
	vramSetBankC(VRAM_C_SUB_BG); 

	SUB_BG0_CR = BG_256_COLOR | BG_TILE_BASE(char_base) | BG_MAP_BASE(screen_base);

	u16* sub_tile = (u16*)CHAR_BASE_BLOCK_SUB(char_base);
	u16* sub_map = (u16*)SCREEN_BASE_BLOCK_SUB(screen_base);

	//95 and 32 show how many characters there are and 32 shows which ASCII character to start, respectively
	//95 is the smaller set of ACSII characters. It usually will start with 32
	consoleInit((u16*)fontData, sub_tile, 95, 32, sub_map, CONSOLE_USE_COLOR255, 8);
    
	//Load the Font Data and Palette stuff here
	for(i = 0; i < fontDataSize; ++i) {
		sub_tile[i] = fontData[i];
	}


	for(i = 0; i < fontPaletteSize; ++i) {
		BG_PALETTE_SUB[i] = fontPalette[i];
	}


	iprintf("Custom Font Demo\n");
	iprintf("   by Poffy\n");
	iprintf("modified by WinterMute\n");
	iprintf("for libnds examples\n");

	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}
