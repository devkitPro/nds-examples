/*		Simple Maxmod example - Song events   

	See http://www.maxmod.org/ref/tut/songevents.html for an explanation of song events.
	
	Example and mod file by Nyarla 
	(modified by Jason Rogers aka Dovoto)
*/




#include <nds.h>
#include <maxmod9.h>

#include "mmsolution.h"		// solution definitions
#include "mmsolution_bin.h"	// solution binary reference 
#include "ball.h"

int spriteDy = 0;
int spriteY = 140;

//---------------------------------------------------------------------------------
// callback function to handle song events
//---------------------------------------------------------------------------------
mm_word myEventHandler( mm_word msg, mm_word param ) {
//---------------------------------------------------------------------------------
	switch( msg ) {

	case MMCB_SONGMESSAGE:	// process song messages

		// if song event 1 is triggered, set sprite's y velocity to make it jump
		if (param == 1) spriteDy = -16;

        break;
		
		case MMCB_SONGFINISHED:	// process song finish message (only triggered in songs played with MM_PLAY_ONCE)
        
		break;
    }
	
	return 0;
}


//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	
	videoSetMode(MODE_0_2D);
	
	//initialize the sprite engine with 1D mapping 32 byte boundary
	//and no external palette support
	oamInit(&oamMain, SpriteMapping_1D_32, false);

	vramSetBankA(VRAM_A_MAIN_SPRITE);

	//allocate some space for the sprite graphics	
	u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);

	//copy in our ball graphics
	dmaCopy(ballTiles, gfx, ballTilesLen);
	dmaCopy(ballPal, SPRITE_PALETTE, ballPalLen);
	
	//--------------------------------------------
	// initialise maxmod using default settings, and 
	// enable interface for a soundbank that is loaded 
	// into memory
	//--------------------------------------------
	mmInitDefaultMem( (mm_addr)mmsolution_bin );

	// setup maxmod to use the song event handler
	mmSetEventHandler( myEventHandler );
	
	// load song
	// values for this function are in the solution header
	mmLoad( MOD_EXAMPLE );

	// start the music playing
	mmStart( MOD_EXAMPLE, MM_PLAY_LOOP );

	while(1) {
		// Sprite accelerates down
		spriteDy += 2;
		
		// sprite falls
		spriteY += spriteDy;
		
		// Floor is arbitrarily set to 140
		if ( spriteY > 140 ) spriteY = 140;
		
		
		oamSet(&oamMain, 			//main graphics engine context
			0,           			//oam index (0 to 127)  
			256/2-16,				//x and y pixel location of the sprite
			spriteY, 			
			0,						//priority, lower renders last (on top)
			0,	//palette index if multiple palettes or the alpha value if bmp sprite	
			SpriteSize_32x32,
			SpriteColorFormat_256Color,
			gfx,				//pointer to the loaded graphics
			-1,	//sprite rotation data  
			false,					//double the size when rotating?
			false,			//hide the sprite?
			false, false, //vflip, hflip
			false	//apply mosaic
			);              
	
		swiWaitForVBlank();
		
		//send the updates to the hardware
		oamUpdate(&oamMain);
	}

	return 0;
}


