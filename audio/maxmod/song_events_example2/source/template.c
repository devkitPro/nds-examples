/*	

	Simple Maxmod example - Song events   

	See http://www.maxmod.org/ref/tut/songevents.html for an explanation of song events.
	
	Example and mod file by Nyarla 
	(modified by Jason Rogers aka Dovoto)
*/



#include <nds.h>
#include <maxmod9.h>

#include "mmsolution.h"		// solution definitions
#include "mmsolution_bin.h"	// solution binary reference 


// a simple sprite structure
typedef struct {
   u16* gfx;
   SpriteSize size;
   SpriteColorFormat format;
   int rotationIndex;
   int paletteAlpha;
   int x;
   int y;
   int dy;
}MySprite;


// create 5 sprites, one for each song event used
MySprite sprites[] = {	{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 20, 96, 0},
						{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 70, 96, 0},
						{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 120, 96, 0},
						{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 170, 96, 0},
						{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 220, 96, 0}	};



//---------------------------------------------------------------------------------
// callback function to handle song events
//---------------------------------------------------------------------------------
mm_word myEventHandler( mm_word msg, mm_word param ) {
//---------------------------------------------------------------------------------

	switch( msg ) {

	case MMCB_SONGMESSAGE:	// process song messages

			if (param==1) sprites[0].dy = -8;
			if (param==2) sprites[1].dy = -8;
			if (param==3) sprites[2].dy = -8;
			if (param==4) sprites[3].dy = -8;
			if (param==5) sprites[4].dy = -8;
		
        break;
		
    case MMCB_SONGFINISHED:	// process song finish message (only triggered in songs played with MM_PLAY_ONCE)
        
		break;
    }
	
	return 0;
}



//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	int i = 0;
	
	videoSetMode(MODE_0_2D);
	videoSetModeSub(0);   // not using subscreen

	lcdMainOnBottom();
	
	//initialize the sprite engine with 1D mapping 128 byte boundary
	//and no external palette support
	oamInit(&oamMain, SpriteMapping_1D_32, false);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	
	for (i = 0; i < 5; i++) {
		//allocate some space for the sprite graphics
		sprites[i].gfx = oamAllocateGfx(&oamMain, sprites[i].size, sprites[i].format);

		//fill each sprite with a different index (2 pixels at a time)
		dmaFillHalfWords( ((i+1)<<8)|(i+1), sprites[i].gfx, 32*32);
	}

	//set indexes to different colours
	SPRITE_PALETTE[1] = RGB15(31,0,0);
	SPRITE_PALETTE[2] = RGB15(0,31,0);
	SPRITE_PALETTE[3] = RGB15(0,0,31);
	SPRITE_PALETTE[4] = RGB15(31,0,31);
	SPRITE_PALETTE[5] = RGB15(0,31,31);

	// initialise maxmod using default settings, and enable interface for soundbank that is loaded into memory
	mmInitDefaultMem( (mm_addr)mmsolution_bin );

	// setup maxmod to use the song event handler
	mmSetEventHandler( myEventHandler );
	
	// load song
	// values for this function are in the solution header
	mmLoad( MOD_EXAMPLE2 );

	// start song playing
	mmStart( MOD_EXAMPLE2, MM_PLAY_LOOP );

	while(1) {
		for (i=0; i < 5; i++) {
			// constantly increase the sprite's y velocity
			sprites[i].dy += 1;
		
			// update the sprite's y position with its y velocity
			sprites[i].y += sprites[i].dy;
		
			// clamp the sprite's y position
			if ( sprites[i].y<72 ) sprites[i].y = 72;
			if ( sprites[i].y>96 ) sprites[i].y = 96;
		
			oamSet(	&oamMain, 					//main graphics engine context
					i,           				//oam index (0 to 127)  
					sprites[i].x,				//x and y pixel location of the sprite
					sprites[i].y, 			
					0,							//priority, lower renders last (on top)
					sprites[i].paletteAlpha,	//palette index 
					sprites[i].size,
					sprites[i].format,
					sprites[i].gfx,				//pointer to the loaded graphics
					sprites[i].rotationIndex,	//sprite rotation data  
					false,						//double the size when rotating?
					false,			//hide the sprite?
					false, false, //vflip, hflip
					false	//apply mosaic
					);              
			}

		swiWaitForVBlank();
		
		//send the updates to the hardware
		oamUpdate(&oamMain);
	}

	return 0;
}


