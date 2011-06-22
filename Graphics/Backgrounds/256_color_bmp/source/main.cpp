#include <nds.h>

#include <stdio.h>

// git adds a nice header we can include to access the data
// this has the same name as the image
#include "drunkenlogo.h"

int main(void)
{
    // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\t256 color bitmap demo");

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	dmaCopy(drunkenlogoBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(drunkenlogoPal, BG_PALETTE, 256*2);

	while(1) {
		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
	}

	return 0;
}
