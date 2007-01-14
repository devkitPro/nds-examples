
#include <nds.h>

#include <nds/arm9/console.h> //basic print funcionality
#include <nds/arm9/trig_lut.h>
#include <stdio.h>


// make file automaticaly makes a header file for access
// to the binary data in any file ending in .bin that is in
// the data folder.  It also links in that data to your project

#include "drunkenlogo_bin.h"
#include "palette_bin.h"


int main(void)
{
	irqInit();
	irqSet(IRQ_VBLANK, 0);

	// set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	// set the sub background up for text display (we could just print to one
	// of the main display text backgrounds just as easily
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE); //sub bg 0 will be used to print text

	// set the first bank as background memory and the third as sub background memory
	// B and D are not used
	vramSetMainBanks(	VRAM_A_MAIN_BG_0x06000000, VRAM_B_LCD,
						VRAM_C_SUB_BG , VRAM_D_LCD);

	// set up text background for text
	SUB_BG0_CR = BG_MAP_BASE(31);

	BG_PALETTE_SUB[255] = RGB15(31,31,31);//by default font will be rendered with color 255

	//consoleInit() is a lot more flexible but this gets you up and running quick
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);



	// set up our bitmap background

	BG3_CR = BG_BMP8_256x256;

	// these are rotation backgrounds so you must set the rotation attributes:
	// these are fixed point numbers with the low 8 bits the fractional part
	// this basicaly gives it a 1:1 translation in x and y so you get a nice flat bitmap
		BG3_XDX = 1 << 8;
		BG3_XDY = 0;
		BG3_YDX = 0;
		BG3_YDY = 1 << 8;
	// our bitmap looks a bit better if we center it so scroll down (256 - 192) / 2
		BG3_CX = 0;
		BG3_CY = 32 << 8;

	dmaCopy(drunkenlogo_bin, BG_GFX, 256*256);
	dmaCopy(palette_bin, BG_PALETTE, 256*2);

		unsigned int angle = 0;

	// the screen origin is at the rotation center...so scroll to the rotation
	// center + a small 32 pixle offset so our image is centered
	s16 scrollX = 0 + 128;
	s16 scrollY = 32 + 96 ;

	//scale is fixed point
	s16 scaleX = 1 << 8;
	s16 scaleY = 1 << 8;

	//this is the screen pixel that the image will rotate about
	s16 rcX = 128;
	s16 rcY = 96;

	while(1)
	{
	// Print status

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\tBG Rotation demo\n");

		iprintf("Angle %3d(actual) %3d(degrees)\n", angle & 0x1FF, (angle & 0x1FF) * 360 / 512);
		iprintf("Scroll  X: %4d Y: %4d\n", scrollX, scrollY);
		iprintf("Rot center X: %4d Y: %4d\n", rcX, rcY);
		iprintf("Scale X: %4d Y: %4d\n", scaleX, scaleY);

	scanKeys();
	u32 keys = keysHeld();

		if( keys & KEY_L ) angle++;
		if( keys & KEY_R ) angle--;
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

	// Compute sin and cos
		s16 s = SIN[angle & 0x1FF] >> 4;
		s16 c = COS[angle & 0x1FF] >> 4;

		swiWaitForVBlank();

	// Set the background registers
		BG3_XDX = ( c * scaleX ) >> 8;
		BG3_XDY = (-s * scaleX ) >> 8;
		BG3_YDX = ( s * scaleY ) >> 8;
		BG3_YDY = ( c * scaleY ) >> 8;

		BG3_CX = (scrollX<<8) - rcX * (c - s);
		BG3_CY = (scrollY<<8) - rcY * (s + c);

		// clear the console screen (ansi escape sequence)
		iprintf("\x1b[2J");

	}
	return 0;
}
