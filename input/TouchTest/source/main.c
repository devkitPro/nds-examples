#include <nds.h>
 
#include <nds/arm9/console.h> //basic print funcionality
#include <stdio.h>
 
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
	unsigned int i;
	
	for(i = 0; i < 128 * sizeof(SpriteEntry) / 4 ; i++) {
		((uint32*)OAM)[i] = ((uint32*)OAMCopy)[i];
	}
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

	powerON(POWER_ALL_2D);

	// Initialise the interrupt system
	irqInit();
	// install our simple vblank handler
	irqSet(IRQ_VBLANK, Vblank);
	// enable the interrupt
	irqEnable(IRQ_VBLANK);
	initOAM();
    //enable vram and map it to the right places
    vramSetMainBanks(   VRAM_A_MAIN_SPRITE,        //A and B maped consecutivly as sprite memory
                        VRAM_B_MAIN_SPRITE,        //this gives us 256KB which is the max
                        VRAM_C_MAIN_BG_0x6000000,  //map C to background memory
                        VRAM_D_LCD                 //not using D
                        ); 
   
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

	// black backdrop
	BG_PALETTE[0]=RGB15(0,0,0);

	BG0_CR = BG_MAP_BASE(31);//use bg0 for the text
	
	BG_PALETTE[255] = RGB15(31,31,31);//by default font rendered with color 255
	
	//consoleInit() is a lot more flexible but this gets you up and running quick
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK(31), (u16*)CHAR_BASE_BLOCK(0), 16);

 
	iprintf("\x1b[4;8HTouch Screen Test");
	iprintf("\x1b[15;4HRight Shoulder toggles");
 
	while(1) {

		swiWaitForVBlank();
		updateOAM();

		// read the button states
		scanKeys();

		// read the touchscreen coordinates
		touch=touchReadXY();
		
		int pressed = keysDown();	// buttons pressed this loop
		int held = keysHeld();		// buttons currently held

		// Right Shoulder button toggles the mode
		if ( pressed & KEY_R) TouchType ^= SINGLE;

		iprintf("\x1b[14;4HTouch mode: %s",TouchType==CONTINUOUS?"CONTINUOUS ":"SINGLE SHOT");

		iprintf("\x1b[6;5HTouch x = %04X, %04X\n", touch.x, touch.px);
		iprintf("\x1b[7;5HTouch y = %04X, %04X\n", touch.y, touch.py);		

		iprintf("\x1b[0;18Hkeys: %08X\n", keysHeld());
		iprintf("\x1b[9;10HFrame %d\n", frame);

		if ( TouchType == SINGLE && !(pressed & KEY_TOUCH) ) continue;

		if ( !(held & KEY_TOUCH) || touch.x == 0 || touch.y == 0) continue;
		
		iprintf("\x1b[12;12H(%d,%d)      ",touch.px,touch.py);

		if ( touch.x > max_x)		max_x = touch.x;
		if ( touch.y > max_y)		max_y = touch.y;
		if ( touch.px > max_px)	max_px = touch.px;
		if ( touch.py > max_py)	max_py = touch.py;

		if ( touch.x < min_x)		min_x = touch.x;
		if ( touch.y < min_y)		min_y = touch.y;
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
