/*---------------------------------------------------------------------------------

Simple extended palette demo.  Creates two sprites on the main display
using palette 0 and palette 1.

-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	int i = 0;

	touchPosition touch;

	videoSetMode(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, true);

	u16* gfx1 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	u16* gfx2 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);

	//------------------------------------------------------------------
	// notice both sprites are filled with color 1
	//------------------------------------------------------------------
	for(i = 0; i < 16 * 16 / 2; i++)
	{
		gfx1[i] = 1 | (1 << 8);
		gfx2[i] = 1 | (1 << 8);
	}

	//------------------------------------------------------------------
	// unlock vram (cannot write to vram while mapped as palette memory)
	//------------------------------------------------------------------
	vramSetBankF(VRAM_F_LCD);

	VRAM_F_EXT_SPR_PALETTE[0][1] = RGB15(31,0,0);
	VRAM_F_EXT_SPR_PALETTE[1][1] = RGB15(0,31,0);

	// set vram to ex palette
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);

	while(1) {

		scanKeys();

		if(keysHeld() & KEY_TOUCH)
			touchRead(&touch);

		oamSet(&oamMain, //main graphics engine context
			0,           //oam index (0 to 127)  
			touch.px, touch.py,   //x and y pixle location of the sprite
			0,                    //priority, lower renders last (on top)
			0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
			SpriteSize_16x16,     
			SpriteColorFormat_256Color, 
			gfx1,                  //pointer to the loaded graphics
			-1,                  //sprite rotation data  
			false,               //double the size when rotating?
			false,			//hide the sprite?
			false, false, //vflip, hflip
			false	//apply mosaic
			);              
		
		
		oamSet(&oamMain, 
			1, 
			SCREEN_WIDTH - touch.px, 
			SCREEN_HEIGHT - touch.py, 
			0, 
			1,  //use second palette
			SpriteSize_16x16, 
			SpriteColorFormat_256Color, 
			gfx2, 
			-1, 
			false, 
			false,			
			false, false, 
			false	
			);              
	
		swiWaitForVBlank();

		
		oamUpdate(&oamMain);
	}

	return 0;
}