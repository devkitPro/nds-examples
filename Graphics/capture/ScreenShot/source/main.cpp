#include <nds.h>

#include <stdio.h>

#include "screenshot.h"

//a global copy of sprite attribute memory
SpriteEntry sprites[128];
//rotation attributes overlap so assign then to the same location
pSpriteRotation spriteRotations = (pSpriteRotation)&sprites[0];


//turn off all the sprites
void initSprites(void) {

	for(int i = 0; i < 128; i++) {

	   sprites[i].attribute[0] = ATTR0_DISABLED;
	   sprites[i].attribute[1] = 0;
	   sprites[i].attribute[2] = 0;
	   sprites[i].filler = 0;
    }
}
//copy our sprite to object attribute memory
void updateOAM(void) {

	DC_FlushRange(sprites,128*sizeof(SpriteEntry));
	dmaCopy(sprites, OAM, 128 * sizeof(SpriteEntry));
}

int main() {
 
    //enable vram and map it to the right places
    vramSetPrimaryBanks(	VRAM_A_MAIN_SPRITE,        //A and B maped consecutivly as sprite memory
                        	VRAM_B_MAIN_SPRITE,        //this gives us 256KB which is the max
                        	VRAM_C_MAIN_BG_0x06000000, //map C to background memory
                        	VRAM_D_LCD                 //not using D
                        ); 
   
   //set the video mode
    videoSetMode(  MODE_0_2D | 
                   DISPLAY_SPR_ACTIVE |    //turn on sprites
                   DISPLAY_BG0_ACTIVE |    //turn on background 0
                   DISPLAY_SPR_1D |        //this is used when in tile mode
                   DISPLAY_SPR_1D_BMP      //and this in bitmap mode
                    );

  	
	consoleInit(0, 0,BgType_Text4bpp, BgSize_T_256x256, 31,0, true, true);
    
    //turn off the sprites
    initSprites();

	
// direct bitmap sprite
// print at using ansi escape sequence \x1b[line;columnH 
	iprintf("\x1b[1;1HDirect Bitmap:");
    sprites[0].attribute[0] = ATTR0_BMP | ATTR0_ROTSCALE_DOUBLE | 10; 
	sprites[0].attribute[1] = ATTR1_SIZE_32 | 20;
	sprites[0].attribute[2] = ATTR2_ALPHA(1)| 0;

	// red 32*32 square for 1d bitmap mode
	for(int i=0;i<32*32;i++)
		SPRITE_GFX[i]=RGB15(31,0,0)|(1<<15); //dont forget alpha bit

// 256 color sprite
// print at using ansi escape sequence \x1b[line;columnH 
	iprintf("\x1b[9;1H256 color:");
	sprites[1].attribute[0] = ATTR0_COLOR_256 | ATTR0_ROTSCALE_DOUBLE | 75;
	sprites[1].attribute[1] = ATTR1_SIZE_32 | 20; // size 64x64, x 10
	sprites[1].attribute[2] = 64;

    // Blue for 256 color sprite
	SPRITE_PALETTE[1]=RGB15(0,0,31);

    // blue 64*64 square for 256 color mode (must write two pixles at time)
	for(int i=0;i<32*16;i++)
		SPRITE_GFX[i+64*16]=(1<<8)|1;

// 16 color sprite
// print at using ansi escape sequence \x1b[line;columnH 
   	iprintf("\x1b[16;1H16 color:");
	sprites[2].attribute[0] = ATTR0_COLOR_16 | ATTR0_ROTSCALE_DOUBLE | 135;
	sprites[2].attribute[1] = ATTR1_SIZE_32 | 20;
	sprites[2].attribute[2] = ATTR2_PALETTE(1) | 96;

    //yellow for 16 color sprite (it is using palette 1 so colors 16-31)
    SPRITE_PALETTE[17]=RGB15(31,31,0);

    // yellow 32*32 square for 16 color mode (must write 4 pixels at a time)
	for(int i=0;i<32*8;i++)
		SPRITE_GFX[i+96*16]=(1<<12)|(1<<8)|(1<<4)|1;



	int angle=0;
    
    //we tied all our sprites to the same rotation attributes (0)
	spriteRotations[0].hdx=256;
	spriteRotations[0].hdy=0;
	spriteRotations[0].vdx=0;
	spriteRotations[0].vdy=256;

	while(1)
	{
		angle+=64;
		
		spriteRotations[0].hdx = cosLerp(angle) >> 4;
		spriteRotations[0].hdy = sinLerp(angle) >> 4;
		spriteRotations[0].vdx = -spriteRotations[0].hdy;
		spriteRotations[0].vdy = spriteRotations[0].hdx;
		
		swiWaitForVBlank();
		
		u32 display_temp = 0;

		scanKeys();
		if(keysDown()&KEY_A) {
		
			screenshotbmp("shot.bmp");

			display_temp=REG_DISPCNT;
			REG_DISPCNT=MODE_FB1;

		}

		if(keysUp()&KEY_A) {
		
			videoSetMode(  MODE_0_2D | 
				DISPLAY_SPR_ACTIVE |    //turn on sprites
				DISPLAY_BG0_ACTIVE |    //turn on background 0
				DISPLAY_SPR_1D |        //this is used when in tile mode
				DISPLAY_SPR_1D_BMP |    //and this in bitmap mode
				DCAP_OFFSET(1));
		}

		
		updateOAM();
	}
}
