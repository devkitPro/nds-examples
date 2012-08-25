/*---------------------------------------------------------------------------------

A very simple window demo which displays a square window

---------------------------------------------------------------------------------*/
#include <nds.h>
#include "drunkenlogo.h"


//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
    
    videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
	
    //enable a background
    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
    
    //use the standard drunken logo
	dmaCopy(drunkenlogoBitmap, bgGetGfxPtr(bg3), drunkenlogoBitmapLen);
    dmaCopy(drunkenlogoPal, BG_PALETTE, drunkenlogoPalLen);

	//enable window 0 
	windowEnable(WINDOW_0);
	
	//enable window 0 on our new background
	bgWindowEnable(bg3, WINDOW_0);

	int x = 60;
	int y = 60;
	int size = 100;

	while(1) 
    {
		
		scanKeys();
		//the code below just moves the window around 
		if(keysHeld() & KEY_UP) y--;
		if(keysHeld() & KEY_DOWN) y++;
		if(keysHeld() & KEY_LEFT) x--;
		if(keysHeld() & KEY_RIGHT) x++;
		
		if(keysHeld() & KEY_A) size--;
		if(keysHeld() & KEY_B) size++;

		if(keysHeld() & KEY_X) 
		{
			bgWindowDisable(bg3, WINDOW_OUT);
			bgWindowEnable(bg3, WINDOW_0);
		}
		if(keysHeld() & KEY_Y)
		{
			bgWindowDisable(bg3, WINDOW_0);
			bgWindowEnable(bg3, WINDOW_OUT);
		}

		if(x < 0) x = 0;
		if(x > SCREEN_WIDTH - 1) x = SCREEN_WIDTH - 1;
		if(y < 0) y = 0;
		if(y > SCREEN_HEIGHT - 1) y = SCREEN_HEIGHT - 1;

		swiWaitForVBlank();

		//set up the boundaries on our window
		windowSetBounds(WINDOW_0, x, y, x + size, y + size);
    }

}