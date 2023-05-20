#include <nds.h>
#include <stdio.h>
#include <string.h>

#include "ball.h"

static enum { CONTINUOUS, SINGLE } TouchType = CONTINUOUS;

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int min_x  = 4096 , min_y  = 4096, max_x  = 0, max_y   = 0;
	int min_px = 4096 , min_py = 4096, max_px = 0 , max_py = 0;
	touchPosition touch;

	// put the main screen on the bottom lcd
	lcdMainOnBottom();

	// set the video mode
	videoSetMode(MODE_0_2D);

	// Sprite initialisation
	oamInit(&oamMain, SpriteMapping_1D_32, false);

	// enable vram and map it to the right places
	vramSetPrimaryBanks(
		VRAM_A_MAIN_BG,     //map A to background memory
		VRAM_B_MAIN_SPRITE, //map B to sprite memory
		VRAM_C_LCD,         //not using C
		VRAM_D_LCD          //not using D
	);

	// load sprite palette
	for(unsigned i = 0; i < 256; i++)
		SPRITE_PALETTE[i] = ((u16*)ballPal)[i];

	// load sprite graphics
	for(unsigned i = 0; i< 32*16; i++)
		SPRITE_GFX[i] = ((u16*)ballTiles)[i];

	// initialise console background
	consoleInit(0, 0,BgType_Text4bpp, BgSize_T_256x256, 31,0, true, true);

	iprintf("\x1b[4;8HTouch Screen Test");
	iprintf("\x1b[15;4HRight Shoulder toggles");

	while(pmMainLoop()) {

		swiWaitForVBlank();
		oamUpdate(&oamMain);

		// read the button states
		scanKeys();

		// read the touchscreen coordinates
		touchRead(&touch);

		int pressed = keysDown();	// buttons pressed this loop
		int held = keysHeld();		// buttons currently held

		// Right Shoulder button toggles the mode
		if ( pressed & KEY_R) TouchType ^= SINGLE;

		// Start button exits the app
		if ( pressed & KEY_START) break;

		iprintf("\x1b[14;4HTouch mode: %s",TouchType==CONTINUOUS?"CONTINUOUS ":"SINGLE SHOT");

		iprintf("\x1b[6;5HTouch x = %04X, %04X\n", touch.rawx, touch.px);
		iprintf("\x1b[7;5HTouch y = %04X, %04X\n", touch.rawy, touch.py);

		iprintf("\x1b[0;18Hkeys: %08lX\n", keysHeld());

		if ( TouchType == SINGLE && !(pressed & KEY_TOUCH) ) continue;

		if ( !(held & KEY_TOUCH) || touch.rawx == 0 || touch.rawy == 0) continue;

		iprintf("\x1b[12;12H(%d,%d)      ",touch.px,touch.py);

		if ( touch.rawx > max_x)		max_x = touch.rawx;
		if ( touch.rawy > max_y)		max_y = touch.rawy;
		if ( touch.px > max_px)	max_px = touch.px;
		if ( touch.py > max_py)	max_py = touch.py;

		if ( touch.rawx < min_x)		min_x = touch.rawx;
		if ( touch.rawy < min_y)		min_y = touch.rawy;
		if ( touch.px < min_px)	min_px = touch.px;
		if ( touch.py < min_py)	min_py = touch.py;

		iprintf("\x1b[0;0H(%d,%d)      ",min_px,min_py);
		iprintf("\x1b[1;0H(%d,%d)      ",min_x,min_y);
		iprintf("\x1b[22;21H(%d,%d)",max_x,max_y);
		iprintf("\x1b[23;23H(%d,%d)",max_px,max_py);

		// Move and display sprite
		oamSet(&oamMain, 0,
			((touch.px - 16) & 0x01FF), // X position
			((touch.py - 16) & 0x00FF), // Y position
			0,
			0,
			SpriteSize_32x32, SpriteColorFormat_256Color,
			SPRITE_GFX,
			-1,
			false, false, false, false, false);
	}

	return 0;
}
