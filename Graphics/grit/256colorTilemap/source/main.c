/*---------------------------------------------------------------------------------

   Simple tilemap demo
   -- WinterMute

---------------------------------------------------------------------------------*/
#include <nds.h>

#include "tilemap.h"

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	// enable the main screen with background 0 active
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);

	// map bank A for use with the background
	vramSetBankA(VRAM_A_MAIN_BG);

	// enable background 0 in 256 color mode with a 256x256 map
	// BG_TILE_BASE changes the offset where tile data is stored
	// BG_MAP_BASE gives the offset to the map data
	BGCTRL[0] = BG_TILE_BASE(1) | BG_MAP_BASE(0) | BG_COLOR_256 | BG_32x32;

	// use dma to copy the tile, map and palette data to VRAM
	// CHAR_BASE_BLOCK gives us the actual address of the tile data
	// SCREEN_BASE_BLOCK does the same thing for maps
	// these should match the BG_TILE_BASE and BG_MAP base numbers above
	dmaCopy(tilemapTiles,(void *)CHAR_BASE_BLOCK(1),tilemapTilesLen);
	dmaCopy(tilemapMap,(void *)SCREEN_BASE_BLOCK(0),tilemapMapLen);
	dmaCopy(tilemapPal,BG_PALETTE,tilemapPalLen);
	// finally, hang around in an infinite loop
	// using swiWaitForVBlank here puts the DS into a low power loop
	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}
