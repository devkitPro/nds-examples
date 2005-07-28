//////////////////////////////////////////////////////////////////////
// Simple consol print demo
// -- dovoto
//////////////////////////////////////////////////////////////////////

#include <nds.h>

#include <nds/arm9/console.h> //basic print funcionality
#include <nds/arm9/rand.h>



void WaitForVblank()
{
	while(DISP_Y!=192);
	while(DISP_Y==192);
}

int main(void)
{
    //set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE); 
	
	//set the sub background up for text display (we could just print to one
	//of the main display text backgrounds just as easily
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE); //sub bg 0 will be used to print text
	
    //set the first two banks as background memory and the third as sub background memory
    //D is not used..if you need a bigger background then you will need to map
    //more vram banks consecutivly (VRAM A-D are all 0x20000 bytes in size)
    vramSetMainBanks(VRAM_A_MAIN_BG_0x6000000, VRAM_B_MAIN_BG_0x6020000, 
                     VRAM_C_SUB_BG , VRAM_D_LCD); 

	////////////////set up text background for text/////////////////////
    SUB_BG0_CR = BG_MAP_BASE(31);
	
	BG_PALETTE_SUB[255] = RGB15(31,31,31);//by default font will be rendered with color 255
	
	//consoleInit() is a lot more flexible but this gets you up and running quick
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);

	consolePrintf("\n\n\tHello DS devers\n");
	consolePrintf("\twww.drunkencoders.com\n");
	consolePrintf("\tdouble buffer demo");
	
	///////////////set up our bitmap background///////////////////////
	
	BG3_CR = BG_BMP16_256x256;
	
	//these are rotation backgrounds so you must set the rotation attributes:
    //these are fixed point numbers with the low 8 bits the fractional part
    //this basicaly gives it a 1:1 translation in x and y so you get a nice flat bitmap
        BG3_XDX = 1 << 8;
        BG3_XDY = 0;
        BG3_YDX = 0;
        BG3_YDY = 1 << 8;
    //our bitmap looks a bit better if we center it so scroll down (256 - 192) / 2 
        BG3_CX = 0;
        BG3_CY = 0;
        
    
	u16* frontBuffer = (u16*)(0x06000000);
	u16* backBuffer =  (u16*)(0x06000000 + 256 * 256 * 2);
	
	//this is just used so we can write red color bits to one frame and green to the 
	//other
	u16 colorMask = 0x1F;
	
    while(1)
    {
        //draw a box
        for(int iy = 60; iy < 196 - 60; iy++)
            for(int ix = 60; ix < 256 - 60; ix++)
                backBuffer[iy * 256 + ix] = (rand() & colorMask) | BIT(15);
                
        WaitForVblank();
        
        //swap
        u16* temp = frontBuffer;
        frontBuffer = backBuffer;
        backBuffer = temp;
        
        //flip 
        //base is 16KB and screen size is 256x256x2 (128KB)
        BG3_CR ^= BG_BMP_BASE( 128 / 16 );
        
        //this will cause red or green bits only to be set and swap each
        //frame
        colorMask ^= 0x3FF;
    }
	return 0;
}
