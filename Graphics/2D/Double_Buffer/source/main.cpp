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
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE); 
	
	//set the first two banks as background memory and the third as sub background memory
	//D is not used..if you need a bigger background then you will need to map
	//more vram banks consecutivly (VRAM A-D are all 0x20000 bytes in size)
	vramSetMainBanks(	VRAM_A_MAIN_BG_0x06000000, VRAM_B_MAIN_BG_0x06020000, 
						VRAM_C_SUB_BG , VRAM_D_LCD); 

	consoleDemoInit();

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\tdouble buffer demo");
	
	while(1);

	// set up our bitmap background
	
	REG_BG3CNT = BG_BMP16_256x256;
	
	// these are rotation backgrounds so you must set the rotation attributes:
    // these are fixed point numbers with the low 8 bits the fractional part
    // this basicaly gives it a 1:1 translation in x and y so you get a nice flat bitmap
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    // our bitmap looks a bit better if we center it so scroll down (256 - 192) / 2
    REG_BG3X = 0;
    REG_BG3Y = 32 << 8;

	
	u16* frontBuffer = (u16*)(0x06000000);
	u16* backBuffer =  (u16*)(0x06000000 + 256 * 256 * 2);
	
	//this is just used so we can write red color bits to one frame and green to the 
	//other
	u16 colorMask = 0x1F;
	
		while(1) {
			//draw a box
			for(int iy = 60; iy < 196 - 60; iy++)
				for(int ix = 60; ix < 256 - 60; ix++) 
					backBuffer[iy * 256 + ix] = (rand() & colorMask) | BIT(15);
								
			swiWaitForVBlank();
				
			//swap
			u16* temp = frontBuffer;
			frontBuffer = backBuffer;
			backBuffer = temp;
				
			//flip 
			//base is 16KB and screen size is 256x256x2 (128KB)
			REG_BG3CNT ^= BG_BMP_BASE( 128 / 16 );
				
			//this will cause red or green bits only to be set and swap each
			//frame
			colorMask ^= 0x3FF;
		}
}
