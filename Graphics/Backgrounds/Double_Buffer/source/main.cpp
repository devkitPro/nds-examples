//---------------------------------------------------------------------------------
// Simple double buffering demo
// -- dovoto
//---------------------------------------------------------------------------------

#include <nds.h>

#include <stdlib.h>
#include <stdio.h>


//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	//set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D); 

	//set the first two banks as background memory and the third as sub background memory
	//D is not used..if you need a bigger background then you will need to map
	//more vram banks consecutivly (VRAM A-D are all 0x20000 bytes in size)
	vramSetPrimaryBanks(	VRAM_A_MAIN_BG_0x06000000, VRAM_B_MAIN_BG_0x06020000, 
		VRAM_C_SUB_BG , VRAM_D_LCD); 

	consoleDemoInit();

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\tdouble buffer demo");


	int bg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);

	u16 colorMask = 0x1F;

	u16* backBuffer = (u16*)bgGetGfxPtr(bg) + 256*256;

	while(1) 
	{
		//draw a box (60,60,196,136)
		for(int iy = 60; iy < 196 - 60; iy++)
			for(int ix = 60; ix < 256 - 60; ix++) 
				backBuffer[iy * 256 + ix] = (rand() & colorMask) | BIT(15);

		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;

		//swap the back buffer to the current buffer
		backBuffer = (u16*)bgGetGfxPtr(bg);

		//swap the current buffer by changing the base. Each base
		//represents 16KB of offset and each screen is 256x256x2 (128KB)
		//this requires a map base seperation of 8 (could get away with smaller
		//as the screen is really only showing 256x192 (96KB or map base 6)
		if(bgGetMapBase(bg) == 8)
		{
			bgSetMapBase(bg, 0);
		}
		else
		{
			bgSetMapBase(bg, 8);
		}

		colorMask ^= 0x3FF;
	}
}
