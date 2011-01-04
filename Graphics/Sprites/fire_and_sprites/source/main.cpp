/*---------------------------------------------------------------------------------
	$Id: main.cpp,v 1.16 2008-11-29 22:58:58 wntrmute Exp $

	-- dovoto

---------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <nds.h>

//include our ball pcx file (auto generated)
#include "ball_pcx.h"

#define NUM_SPRITES 128	

SpriteEntry OAMCopySub[128];

//simple sprite struct
typedef struct {
	int x,y;			// screen co-ordinates 
	int dx, dy;			// velocity
	SpriteEntry* oam;	// pointer to the sprite attributes in OAM
	int gfxID; 			// graphics lovation
}Sprite;


//---------------------------------------------------------------------------------
void MoveSprite(Sprite* sp) {
//---------------------------------------------------------------------------------
	int x = sp->x >> 8;
	int y = sp->y >> 8;

	sp->oam->x = x;
	sp->oam->y = y;

} 



//---------------------------------------------------------------------------------
void initOAM(void) {
//---------------------------------------------------------------------------------
	int i;

	for(i = 0; i < 128; i++) {
		OAMCopySub[i].attribute[0] = ATTR0_DISABLED;
	}	
}

//---------------------------------------------------------------------------------
void updateOAM(void) {
//---------------------------------------------------------------------------------
	memcpy(OAM_SUB, OAMCopySub, 128 * sizeof(SpriteEntry));
}



//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	
	uint16* back = VRAM_A;
	uint16* front = VRAM_B;

	Sprite sprites[NUM_SPRITES];

	int i, delta = 0;
	int ix = 0;
	int iy = 0;
	int screen = 1;
	uint16* map0 = (uint16*)SCREEN_BASE_BLOCK_SUB(1);
	uint16* map1 = (uint16*)SCREEN_BASE_BLOCK_SUB(2);
	uint16 red;

	//set main display to render directly from the frame buffer
	videoSetMode(MODE_FB1);
	
	//set up the sub display
	videoSetModeSub(MODE_0_2D | 
					DISPLAY_SPR_1D_LAYOUT | 
					DISPLAY_SPR_ACTIVE | 
					DISPLAY_BG0_ACTIVE |
					DISPLAY_BG1_ACTIVE );
	
	//vram banks are somewhat complex
	vramSetPrimaryBanks(VRAM_A_LCD, VRAM_B_LCD, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);	

	sImage ball;

	//load our ball pcx file into an image
	loadPCX((u8*)ball_pcx, &ball);
	
	//tile it so it is usefull as sprite data
	imageTileData(&ball);

	// Sprite initialisation
	for(i = 0; i < 256; i++)
		SPRITE_PALETTE_SUB[i] = ball.palette[i];

	for(i = 0; i< 32*16; i++)
		SPRITE_GFX_SUB[i] = ball.image.data16[i];
	
	//turn off sprites
	initOAM();

	for(i = 0; i < NUM_SPRITES; i++) {
		//random place and speed
		sprites[i].x = rand() & 0xFFFF;
		sprites[i].y = rand() & 0x7FFF;
		sprites[i].dx = (rand() & 0xFF) + 0x100;
		sprites[i].dy = (rand() & 0xFF) + 0x100;
	
		if(rand() & 1)
			sprites[i].dx = -sprites[i].dx;
		if(rand() & 1)
			sprites[i].dy = -sprites[i].dy;
	
		sprites[i].oam = &OAMCopySub[i];
		sprites[i].gfxID = 0;
	
		//set up our sprites OAM entry attributes
		sprites[i].oam->attribute[0] = ATTR0_COLOR_256 | ATTR0_SQUARE;  
		sprites[i].oam->attribute[1] = ATTR1_SIZE_32;
		sprites[i].oam->attribute[2] = sprites[i].gfxID;
	}

	//set up two backgrounds to scroll around
	REG_BG0CNT_SUB = BG_COLOR_256 | (1 << MAP_BASE_SHIFT);
	REG_BG1CNT_SUB = BG_COLOR_256 | (2 << MAP_BASE_SHIFT);

	BG_PALETTE_SUB[0] = RGB15(10,10,10);
	BG_PALETTE_SUB[1] = RGB15(0,16,0);
	BG_PALETTE_SUB[2] = RGB15(0,0,31);
	
	//load the maps with alternating tiles (0,1 for bg0 and 0,2 for bg1)
	for(iy = 0; iy < 32; iy++) {
		for(ix = 0; ix <32; ix++) {
			map0[iy * 32 + ix] = (ix ^ iy) & 1;
			map1[iy * 32 + ix] = ((ix ^ iy) & 1)<<1;
		}
	}    

	//fill 2 tiles with different colors
	for(i = 0; i < 64 / 2; i++) {
		BG_GFX_SUB[i+32] = 0x0101;
		BG_GFX_SUB[i+32+32] = 0x0202;
	}	

	while (1) {
		//scroll the background
		REG_BG0HOFS = delta ;
		REG_BG0VOFS = delta++ ;
		
		//move the sprites
		for(i = 0; i < NUM_SPRITES; i++) {
			sprites[i].x += sprites[i].dx;
			sprites[i].y += sprites[i].dy;
			
			//check for collision with the screen boundries
			if(sprites[i].x < (1<<8) || sprites[i].x > (247 << 8))
				sprites[i].dx = -sprites[i].dx;

			if(sprites[i].y < (1<<8) || sprites[i].y > (182 << 8))
				sprites[i].dy = -sprites[i].dy;
			
			//reposition the sprites
			MoveSprite(&sprites[i]);
		}
		

	
		//do the plasma/fire
		for(ix = 0; ix < SCREEN_WIDTH; ix++) {
			back[ix + SCREEN_WIDTH * (SCREEN_HEIGHT - 1)] = rand()& 0xFFFF;
			back[ix + SCREEN_WIDTH * (SCREEN_HEIGHT - 2)] = rand()& 0xFFFF;
		}

		back++;
		
		for(iy = 1; iy < SCREEN_HEIGHT - 2 ; iy++) {
			for(ix = 1; ix < SCREEN_WIDTH - 1; ix++)  {
				red = 0;

				red += front[0];
				red += front[2];
	
				front += SCREEN_WIDTH;

				red += front[0];
				red += front[1];
				red += front[2];
				
				front += SCREEN_WIDTH;

				red += front[0];
				red += front[1];
				red += front[2];

				front -= (2 * SCREEN_WIDTH) - 1;	

				back[0] =  (red >> 3);	
				back++;
			}
			back += 2;
			front += 2;
		
		}

		swiWaitForVBlank();
		
		updateOAM();

		//flip screens
		if(screen) {
			videoSetMode(MODE_FB1);
			front = VRAM_B;
			back = VRAM_A;
			screen = 0;
		} else {
			videoSetMode(MODE_FB0);	
			front = VRAM_A;
			back = VRAM_B;
			screen = 1;
		}
	}    
	return 0;
}

