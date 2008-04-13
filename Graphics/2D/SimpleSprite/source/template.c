/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int i = 0;
   touchPosition touch;

	irqInit();
	irqEnable(IRQ_VBLANK);

   videoSetMode(MODE_0_2D);
   videoSetModeSub(MODE_0_2D);

   vramSetBankA(VRAM_A_MAIN_SPRITE);
   vramSetBankD(VRAM_D_SUB_SPRITE);

   oamInit(&oamMain, SpriteMapping_1D_32, false);
   oamInit(&oamSub, SpriteMapping_1D_32, false);

   u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
   u16* gfxSub = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);

   for(i = 0; i < 16 * 16 / 2; i++)
   {
      gfx[i] = 1 | (1 << 8);
      gfxSub[i] = 1 | (1 << 8);
   }

   SPRITE_PALETTE[1] = RGB15(31,0,0);
   SPRITE_PALETTE_SUB[1] = RGB15(31,0,0);

	while(1) {

      scanKeys();

      if(keysHeld() & KEY_TOUCH)
         touchRead(&touch);

      oamSet(&oamMain, 0, touch.px, touch.py, 0, SpriteSize_16x16, SpriteColorFormat_256Color, gfx, false);
      oamSet(&oamSub, 0, touch.px, touch.py, 0, SpriteSize_16x16, SpriteColorFormat_256Color, gfx, false);

		swiWaitForVBlank();

      oamUpdate(&oamMain);
      oamUpdate(&oamSub);
	}

	return 0;
}
