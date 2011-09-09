#include <nds.h>

#include <RotBackgrounds.h>
#include <TextBackgrounds.h>
#include <Multilayer.h>

#include <stdio.h>
#include <stdlib.h>

void scroll(int id, int width, int height);

void advMosaic(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(Layer256x256Map, bgGetMapPtr(bg),  Layer256x256MapLen);
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	
	bgMosaicEnable(bg);
	
	int keys = 0;
	int mosaic_x = 0;
	int mosaic_y = 0;
	
	while(!(keys & KEY_B))
	{
		scanKeys();
		keys = keysDown();
		
		if(keys & KEY_UP) mosaic_y--;
		if(keys & KEY_DOWN) mosaic_y++;
	
		if(keys & KEY_LEFT) mosaic_x--;
		if(keys & KEY_RIGHT) mosaic_x++;

		if(mosaic_x > 15) mosaic_x = 15;
		if(mosaic_x < 0) mosaic_x = 0;
		
		if(mosaic_y > 15) mosaic_y = 15;
		if(mosaic_y < 0) mosaic_y = 0;

		
		swiWaitForVBlank();
		
		bgSetMosaic(mosaic_x, mosaic_y);
		
		consoleClear();
		iprintf("Press B to exit\n");
		iprintf("DX: %d  DY: %d", mosaic_x, mosaic_y);
		
	}
}

void advRotating(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_ExRotation, BgSize_ER_256x256, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	dmaCopy(Layer256x256Map, bgGetMapPtr(bg),  Layer256x256MapLen);
	
	bgMosaicEnable(bg);
	
	int keys = 0;
	int angle = 0;
	int center_x = 0;
	int center_y = 0;
	
	while(!(keys & KEY_B))
	{
		scanKeys();
		keys = keysHeld();
		
		if(keys & KEY_UP) center_y--;
		if(keys & KEY_DOWN) center_y++;
	
		if(keys & KEY_LEFT) center_x--;
		if(keys & KEY_RIGHT) center_x++;

		if(keys & KEY_L) angle-=40;
		if(keys & KEY_R) angle+=40;

		if(center_x > 256) center_x = 256;
		if(center_x < 0) center_x = 0;
		
		if(center_y > 192) center_y = 192;
		if(center_y < 0) center_y = 0;

		
		swiWaitForVBlank();
		
		bgSetRotate(bg, angle);
		bgSetScroll(bg, center_x, center_y);
		bgSetCenter(bg, center_x, center_y);
		bgUpdate();

		consoleClear();
		iprintf("Press B to exit.n");
		iprintf("Angle: %d \ncenter X: %d  center Y: %d", angleToDegrees(angle), center_x, center_y);
		
	}
}
void advScaling(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_ExRotation, BgSize_ER_256x256, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	dmaCopy(Layer256x256Map, bgGetMapPtr(bg),  Layer256x256MapLen);
	
	bgMosaicEnable(bg);
	
	int keys = 0;

	int scale_x = 1 << 8;
	int scale_y = 1 << 8;
	
	while(!(keys & KEY_B))
	{
		scanKeys();
		keys = keysHeld();
		
		if(keys & KEY_UP) scale_y++;
		if(keys & KEY_DOWN) scale_y--;
	
		if(keys & KEY_LEFT) scale_x++;
		if(keys & KEY_RIGHT) scale_x--;

    	swiWaitForVBlank();
		
		bgSetScale(bg, scale_x , scale_y );
		
		bgUpdate();

		consoleClear();
		iprintf("Press B to exit.n");
		iprintf("scale X: %d  scale Y: %d", scale_x, scale_y);
		
	}
}
void advExtendedPalette(void)
{
  /*
  When extended palettes are enabled all tiled backgrounds which utilize 
  16 bit map entries will use extended palettes.  Everything else will continue
  to use standard palette memory.

  Each tile on the screen may chose one of 16 256-color palettes.  Each background 
  has its own set of 16 palettes meaning you can have 4*16*256 colors on screen 

  Each background uses 8K of palette memory starting at the base of the vram bank
  you allocate (which bank is up to you within limits, see the vram usage table
  to determine which banks can be mapped for textures).  These 8K blocks are often
  refered to as "slots" with each background getting its own slot.  

   By default, Background 0 uses slot 0 ... Background 3 uses slot 3.  It is possible
   to assign Background 0 to slot 2 and Background 1 to slot 3 (only these two are configurable)
  
  For more information: <a href="http://nocash.emubase.de/gbatek.htm#dsvideoextendedpalettes">gbatek</a>
  */

  u16* paletteSlots[4] = 
  {
    &VRAM_E[0*16*256], &VRAM_E[1*16*256], &VRAM_E[2*16*256], &VRAM_E[3*16*256]
  };

  videoSetMode(MODE_0_2D);
  vramSetBankA(VRAM_A_MAIN_BG);

  //enable extended palettes for background. Once on, the standard BG_PALETTE will 
  //be ignored for tiled backgrounds with 16 bit map entries (everything else still uses
  //the standard palette)
  bgExtPaletteEnable();
  
  int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(Layer256x256Map, bgGetMapPtr(bg),  Layer256x256MapLen);
  
  //lock vram E as we are going to use it for 
  //extended palettes (see documentation for which vram 
  //banks can be utilized).  You cannot write to it once
  //it is mapped to the GPU.  These means you should
  //not update your ext palettes outside of a blank period
  vramSetBankE(VRAM_E_LCD);
  dmaCopy(TextBackgroundsPal, paletteSlots[0], sizeof(TextBackgroundsPal));
  
  //tell the GPU to use vram E as extended palette memory
  vramSetBankE(VRAM_E_BG_EXT_PALETTE);

	scroll(bg, 256, 256);
  

}

void advMultipleLayers(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	//initialize the backgrounds
	int bg1 = bgInit(0, BgType_Text8bpp, BgSize_ER_256x256, 0,1);
	int bg2 = bgInit(1, BgType_Text8bpp, BgSize_ER_256x256, 1,1);
	int bg3 = bgInit(2, BgType_ExRotation, BgSize_ER_256x256, 2,1);
	
	//make sure the floor is on the bottom (by default hardware layer 0 will be rendered last)
	bgSetPriority(bg1, 3);
	bgSetPriority(bg2, 2);
	bgSetPriority(bg3, 1);
	
	//they all share tiles and a palette
	dmaCopy(MultilayerTiles, bgGetGfxPtr(bg1), sizeof(MultilayerTiles));
	dmaCopy(MultilayerPal, BG_PALETTE, sizeof(MultilayerPal));

	//all we need to do is copy in the maps
	dmaCopy(Layer_1Map, bgGetMapPtr(bg1),  Layer_1MapLen);
	dmaCopy(Layer_2Map, bgGetMapPtr(bg2),  Layer_2MapLen);
	dmaCopy(Layer_3Map, bgGetMapPtr(bg3),  Layer_3MapLen);
	
	int keys = 0;
	bool bg1_hidden = false;
	bool bg2_hidden = false;
	bool bg3_hidden = false;

	while(!(keys & KEY_B))
	{
		scanKeys();
		
		keys = keysDown();
		
		if(keys & KEY_UP) bg1_hidden = !bg1_hidden;
		if(keys & KEY_DOWN) bg2_hidden = !bg2_hidden;
		if(keys & KEY_LEFT) bg3_hidden = !bg3_hidden;
		
		swiWaitForVBlank();
		
		bg1_hidden ? bgHide(bg1) : bgShow(bg1);
		bg2_hidden ? bgHide(bg2) : bgShow(bg2);
		bg3_hidden ? bgHide(bg3) : bgShow(bg3);
		
		consoleClear();

		iprintf("Press UP DOWN LEFT to toggle the layers\n\n");
		iprintf("Floor (UP): %s\n", bg1_hidden ? "hidden" : "displayed");
		iprintf("Walls (DOWN): %s\n", bg2_hidden ? "hidden" : "displayed");
		iprintf("Decorations (LEFT): %s\n", bg3_hidden ? "hidden" : "displayed");
	}
}
