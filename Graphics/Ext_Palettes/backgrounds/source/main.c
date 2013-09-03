#include <nds.h>
#include "devkitlogo.h"
#include "drunkenlogo.h"

int main(int argc, char *argv[]) {
  int bg[4], i, frames = 0;

  // initialize video
  videoSetMode(MODE_0_2D);
  videoSetModeSub(MODE_0_2D);
  vramSetBankA(VRAM_A_MAIN_BG);
  vramSetBankC(VRAM_C_SUB_BG);

  // enable extended palettes
  bgExtPaletteEnable();
  bgExtPaletteEnableSub();

  // initialize backgrounds
  // extended palettes only work on 8bpp tiled bg's with 16-bit map entries
  // this means you can only use BgType_Text8bpp or BgType_ExRotation
  // http://mtheall.com/vram.html#T0=1&NT0=192&MB0=6&TB0=0&S0=0&T1=1&NT1=576&MB1=7&TB1=1&S1=0
  bg[0] = bgInit   (0, BgType_Text8bpp, BgSize_T_256x256, 6, 0);
  bg[1] = bgInit   (1, BgType_Text8bpp, BgSize_T_256x256, 7, 1);
  // we're using identical settings on the sub screen
  bg[2] = bgInitSub(0, BgType_Text8bpp, BgSize_T_256x256, 6, 0);
  bg[3] = bgInitSub(1, BgType_Text8bpp, BgSize_T_256x256, 7, 1);

  // copy graphics to vram
  dmaCopy(devkitlogoTiles,  bgGetGfxPtr(bg[0]), devkitlogoTilesLen);
  dmaCopy(drunkenlogoTiles, bgGetGfxPtr(bg[1]), drunkenlogoTilesLen);
  // same for sub engine
  dmaCopy(devkitlogoTiles,  bgGetGfxPtr(bg[2]), devkitlogoTilesLen);
  dmaCopy(drunkenlogoTiles, bgGetGfxPtr(bg[3]), drunkenlogoTilesLen);

  // copy maps to vram
  dmaCopy(devkitlogoMap,  bgGetMapPtr(bg[0]), devkitlogoMapLen);
  dmaCopy(drunkenlogoMap, bgGetMapPtr(bg[1]), drunkenlogoMapLen);
  // same for sub engine
  dmaCopy(devkitlogoMap,  bgGetMapPtr(bg[2]), devkitlogoMapLen);
  dmaCopy(drunkenlogoMap, bgGetMapPtr(bg[3]), drunkenlogoMapLen);

  // you can only access extended palettes in LCD mode
  vramSetBankE(VRAM_E_LCD); // for main engine
  vramSetBankH(VRAM_H_LCD); // for sub engine

  // copy palettes to extended palette area
  // there are 16 256-color palettes per bg
  // use '-mp #' to make grit use # for the slot number
  // we used '-mp 12' for drunkenlogo for demonstrative purposes
  dmaCopy(devkitlogoPal,  &VRAM_E_EXT_PALETTE[0][0],  devkitlogoPalLen);  // bg 0, slot 0
  dmaCopy(drunkenlogoPal, &VRAM_E_EXT_PALETTE[1][12], drunkenlogoPalLen); // bg 1, slot 12
  // same for sub engine
  dmaCopy(devkitlogoPal,  &VRAM_H_EXT_PALETTE[0][0],  devkitlogoPalLen);  // bg 0, slot 0
  dmaCopy(drunkenlogoPal, &VRAM_H_EXT_PALETTE[1][12], drunkenlogoPalLen); // bg 1, slot 12

  // map vram banks to extended palettes
  // http://mtheall.com/banks.html#A=MBG0&C=MBG2&E=BGEPAL&H=SBGEPAL
  vramSetBankE(VRAM_E_BG_EXT_PALETTE);     // for main engine
  vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE); // for sub engine

  do {
    swiWaitForVBlank();
    frames++;
    bgUpdate();
    scanKeys();

    // let's make it obvious there are multiple backgrounds
    for(i = 0; i < 8; i++)
      bgSetScroll(i, frames/((i&3)+1), frames/((i&3)+1));
  } while(!keysDown());

  return 0;
}
