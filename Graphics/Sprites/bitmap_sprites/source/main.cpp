#include <nds.h>

#include <stdio.h>

//a simple sprite structure 
//it is generally preferred to separate your game object
//from OAM
typedef struct  
{
   u16* gfx;
   SpriteSize size;
   SpriteColorFormat format;
   int rotationIndex;
   int paletteAlpha;
   int x;
   int y;
}MySprite;

int main(int argc, char** argv) {
	
   //three sprites of differing color format
   MySprite sprites[] = {
      {0, SpriteSize_32x32, SpriteColorFormat_Bmp, 0, 15, 20, 15},
      {0, SpriteSize_32x32, SpriteColorFormat_256Color, 0, 0, 20, 80},
      {0, SpriteSize_32x32, SpriteColorFormat_16Color, 0, 1, 20, 136}
   };

   videoSetModeSub(MODE_0_2D);

   consoleDemoInit();
   
   //initialize the sub sprite engine with 1D mapping 128 byte boundary
   //and no external palette support
   oamInit(&oamSub, SpriteMapping_Bmp_1D_128, false);

  
   vramSetBankD(VRAM_D_SUB_SPRITE);
   
   //allocate some space for the sprite graphics
   for(int i = 0; i < 3; i++)
      sprites[i].gfx = oamAllocateGfx(&oamSub, sprites[i].size, sprites[i].format);
   
   //ugly positional printf
   iprintf("\x1b[1;1HDirect Bitmap:");
   iprintf("\x1b[9;1H256 color:");
   iprintf("\x1b[16;1H16 color:");
   
   //fill bmp sprite with the color red
   dmaFillHalfWords(ARGB16(1,31,0,0), sprites[0].gfx, 32*32*2);
   //fill the 256 color sprite with index 1 (2 pixels at a time)
   dmaFillHalfWords((1<<8)|1, sprites[1].gfx, 32*32);
   //fill the 16 color sprite with index 1 (4 pixels at a time)
   dmaFillHalfWords((1<<12)|(1<<8)|(1<<4)|1, sprites[2].gfx, 32*32 / 2);

   //set index 1 to blue...this will be the 256 color sprite
   SPRITE_PALETTE_SUB[1] = RGB15(0,31,0);
   //set index 17 to green...this will be the 16 color sprite
   SPRITE_PALETTE_SUB[16 + 1] = RGB15(0,0,31);

   int angle = 0;

   while(1) {
      for(int i = 0; i < 3; i++) {
         oamSet(
         &oamSub, //sub display 
         i,       //oam entry to set
         sprites[i].x, sprites[i].y, //position 
         0, //priority
		 sprites[i].paletteAlpha, //palette for 16 color sprite or alpha for bmp sprite
         sprites[i].size, 
		 sprites[i].format, 
		 sprites[i].gfx, 
		 sprites[i].rotationIndex, 
         true, //double the size of rotated sprites
         false, //don't hide the sprite
		 false, false, //vflip, hflip
		 false //apply mosaic
         );
      }

      oamRotateScale(&oamSub, 0, angle, (1 << 8), (1<<8));

	  angle += 64;

      swiWaitForVBlank();

      //send the updates to the hardware
      oamUpdate(&oamSub);
   }
   return 0;
}


