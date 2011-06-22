
#include <nds.h>

#include <nds/arm9/console.h> //basic print funcionality
#include <nds/arm9/trig_lut.h>
#include <stdio.h>


// make file automaticaly makes a header file for access
// to the binary data in any file ending in .bin that is in
// the data folder.  It also links in that data to your project

#include "drunkenlogo_bin.h"
#include "palette_bin.h"


int main(void) {

	videoSetMode(MODE_5_2D );

	vramSetBankA(VRAM_A_MAIN_BG);

	consoleDemoInit();


	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);


	dmaCopy(drunkenlogo_bin, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(palette_bin, BG_PALETTE, 256*2);

	s16 angle = 0;

	// the screen origin is at the rotation center...so scroll to the rotation
	// center + a small 32 pixle offset so our image is centered
	s16 scrollX = 128;
	s16 scrollY = 128 ;

	//scale is fixed point
	s16 scaleX = 1 << 8;
	s16 scaleY = 1 << 8;

	//this is the screen pixel that the image will rotate about
	s16 rcX = 128;
	s16 rcY = 96;

	while(1) {
		// Print status

		iprintf("\n\n\tHello DS devers\n");
		iprintf("\twww.drunkencoders.com\n");
		iprintf("\tBG Rotation demo\n");

		iprintf("Angle %3d(actual) %3d(degrees)\n", angle, (angle * 360) / (1<<15));
		iprintf("Scroll  X: %4d Y: %4d\n", scrollX, scrollY);
		iprintf("Rot center X: %4d Y: %4d\n", rcX, rcY);
		iprintf("Scale X: %4d Y: %4d\n", scaleX, scaleY);

		scanKeys();
		u32 keys = keysHeld();

		if( keys & KEY_L ) angle+=20;
		if( keys & KEY_R ) angle-=20;
		if( keys & KEY_LEFT ) scrollX++;
		if( keys & KEY_RIGHT ) scrollX--;
		if( keys & KEY_UP ) scrollY++;
		if( keys & KEY_DOWN ) scrollY--;
		if( keys & KEY_A ) scaleX++;
		if( keys & KEY_B ) scaleX--;
		if( keys & KEY_START ) rcX ++;
		if( keys & KEY_SELECT ) rcY++;
		if( keys & KEY_X ) scaleY++;
		if( keys & KEY_Y ) scaleY--;


		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;

		bgSetCenter(bg3, rcX, rcY);
		bgSetRotateScale(bg3, angle, scaleX, scaleY);
		bgSetScroll(bg3, scrollX, scrollY);
		bgUpdate();

		// clear the console screen (ansi escape sequence)
		iprintf("\x1b[2J");

	}
	return 0;
}
