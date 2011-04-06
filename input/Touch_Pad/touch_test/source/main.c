#include <nds.h>
#include <stdio.h>
#include <string.h>

SpriteEntry OAMCopy[128];

#include "ballpalette_bin.h"
#include "balldata_bin.h"

//---------------------------------------------------------------------------------
void initOAM(void) {
//---------------------------------------------------------------------------------
	int i;

	for(i = 0; i < 128; i++) {
		OAMCopy[i].attribute[0] = ATTR0_DISABLED;
	}	
}

//---------------------------------------------------------------------------------
void updateOAM(void) {
//---------------------------------------------------------------------------------
	
	memcpy(OAM, OAMCopy, 128 * sizeof(SpriteEntry));
}


volatile int frame = 0;

enum { CONTINUOUS, SINGLE } TouchType = CONTINUOUS;

//---------------------------------------------------------------------------------
void Vblank() {
//---------------------------------------------------------------------------------
	frame++;
}
 
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int min_x  = 4096 , min_y  = 4096, max_x  = 0, max_y   = 0;
	int min_px = 4096 , min_py = 4096, max_px = 0 , max_py = 0;
	touchPosition touch;

	// put the main screen on the bottom lcd
	lcdMainOnBottom();

	initOAM();
   
   //set the video mode
    videoSetMode(  MODE_0_2D | 
                   DISPLAY_SPR_ACTIVE |		//turn on sprites
                   DISPLAY_BG0_ACTIVE |		//turn on background 0
                   DISPLAY_SPR_1D			//this is used when in tile mode
                    );

	int i;
	
	// Sprite initialisation
	for(i = 0; i < 256; i++)
		SPRITE_PALETTE[i] = ((u16*)ballpalette_bin)[i];

	for(i = 0; i< 32*16; i++)
		SPRITE_GFX[i] = ((u16*)balldata_bin)[i];

 
	consoleInit(0, 0,BgType_Text4bpp, BgSize_T_256x256, 31,0, true, true); 
 
	iprintf("\x1b[4;8HTouch Screen Test");
	iprintf("\x1b[15;4HRight Shoulder toggles");
 
	while(1) {

		swiWaitForVBlank();
		updateOAM();

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

		iprintf("\x1b[0;18Hkeys: %08X\n", keysHeld());
		iprintf("\x1b[9;10HFrame %d\n", frame);

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

		OAMCopy[0].attribute[2] = 0;
		OAMCopy[0].attribute[1] = ATTR1_SIZE_32 |((touch.px - 16) & 0x01FF);
		OAMCopy[0].attribute[0] = ATTR0_COLOR_256 | ATTR0_SQUARE | ((touch.py -16) & 0x00FF);
		
	}
	
 
	return 0;
}

