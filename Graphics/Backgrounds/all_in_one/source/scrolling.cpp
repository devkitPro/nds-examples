#include <nds.h>

#include <RotBackgrounds.h>
#include <TextBackgrounds.h>
#include <Multilayer.h>

#include <stdio.h>
#include <stdlib.h>

//reusable scroll function to allow the user to explore 
//the maps somewhat
void scroll(int id, int width, int height)
{
   int keys = 0;
   int sx = 0;
   int sy = 0;

   while(!(keys & KEY_B))
   {
      scanKeys();

      keys = keysHeld();

      if(keys & KEY_UP) sy--;
      if(keys & KEY_DOWN) sy++;
      if(keys & KEY_LEFT) sx--;
      if(keys & KEY_RIGHT) sx++;

      if(sx < 0) sx = 0;
      if(sx >= width - 256) sx = width - 1 - 256;
      if(sy < 0) sy = 0;
      if(sy >= height - 192) sy = height - 1 - 192;

      swiWaitForVBlank();

      bgSetScroll(id, sx, sy);

	  bgUpdate();

      consoleClear();
      iprintf("Scroll x: %d Scroll y: %d\n", sx, sy);
      iprintf("Press 'B' to exit");
   }
}


void scrollText(void)
{
   int keys = 0;
   int sx = 0;
   int sy = 0;
   int width = 256;
   int height = 512;

   //set up a simple text background
   videoSetMode(MODE_0_2D);
   vramSetBankA(VRAM_A_MAIN_BG);

   int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x512, 0,1);

   dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
   dmaCopy(Layer256x512Map, bgGetMapPtr(bg),  Layer256x512MapLen);
   dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));

   while(!(keys & KEY_B))
   {
      scanKeys();

      keys = keysHeld();

      if(keys & KEY_UP) sy--;
      if(keys & KEY_DOWN) sy++;
      if(keys & KEY_LEFT) sx--;
      if(keys & KEY_RIGHT) sx++;

      //clamp the scroll value to the map width and height
      if(sx < 0) sx = 0;
      if(sx >= width - 256) sx = width - 1 - 256;
      if(sy < 0) sy = 0;
      if(sy >= height - 192) sy = height - 1 - 192;

      swiWaitForVBlank();

      //normally would call bgSetScroll(id, sx, sy) here
      //but to demonstrate the hardware difference between 
      //scrolling rotation and text backgrounds we will use
      //direct register access

      REG_BG0HOFS = sx;
      REG_BG0VOFS = sy;

      consoleClear();
      iprintf("Scroll x: %d Scroll y: %d\n", sx, sy);
      iprintf("Press 'B' to exit");
   }
}
void scrollRotation(void)
{
   int keys = 0;
   int sx = 0;
   int sy = 0;
   int width = 512;
   int height = 512;

   videoSetMode(MODE_5_2D);
   vramSetBankA(VRAM_A_MAIN_BG);

   int bg = bgInit(3, BgType_ExRotation, BgSize_ER_512x512, 0,1);

   dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
   dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
   dmaCopy(Layer512x512Map, bgGetMapPtr(bg),  Layer512x512MapLen);

   while(!(keys & KEY_B))
   {
      scanKeys();

      keys = keysHeld();

      if(keys & KEY_UP) sy--;
      if(keys & KEY_DOWN) sy++;
      if(keys & KEY_LEFT) sx--;
      if(keys & KEY_RIGHT) sx++;

      //clamp the scroll value to the map width and height
      if(sx < 0) sx = 0;
      if(sx >= width - 256) sx = width - 1 - 256;
      if(sy < 0) sy = 0;
      if(sy >= height - 192) sy = height - 1 - 192;

      swiWaitForVBlank();

      //normally would call bgSetScroll(id, sx, sy) here
      //but to demonstrate the hardware difference between 
      //scrolling rotation and text backgrounds we will use
      //direct register access

      REG_BG3X = sx << 8;
      REG_BG3Y = sy << 8;

      consoleClear();
      iprintf("Scroll x: %d Scroll y: %d\n", sx, sy);
      iprintf("Press 'B' to exit");
   }


}
void scrollVertical(void)
{
   videoSetMode(MODE_0_2D);
   vramSetBankA(VRAM_A_MAIN_BG);

   int scroll_y = 0;

   int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);

   dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
   dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));

   u16* map = (u16*)bgGetMapPtr(bg);

   dmaCopy(Layer256x512Map, map, 32*32*2);

   int keys = 0;
   int offset = 0;

   while(!(keys & KEY_B))
   {
      scanKeys();

      keys = keysHeld();

      swiWaitForVBlank();


      if(keys & KEY_UP)
      {
         offset = scroll_y / 8 - 1;

         dmaCopy(&Layer256x512Map[(offset & 63) * 32], &map[(offset & 31) * 32], 32 * 2);

         scroll_y--;
      }
      if(keys & KEY_DOWN)		
      {
         offset = scroll_y / 8 + 24;

         dmaCopy(&Layer256x512Map[(offset & 63) * 32], &map[(offset & 31) * 32], 32 * 2);

         scroll_y++;
      }

      bgSetScroll(bg, 0, scroll_y);
	  bgUpdate();
   }

}
void scrollHorizontalText(void)
{
   videoSetMode(MODE_5_2D);
   vramSetBankA(VRAM_A_MAIN_BG);

   int scroll_x = 0;

   int bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x256, 0,1);

   dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
   dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));

   u16* map = (u16*)bgGetMapPtr(bg);


   for(int iy = 0; iy < 24; iy++)
      dmaCopy(&Layer512x256Map[iy * 64], &map[iy * 32], 32*2);

   int keys = 0;
   int layerOffset = 0;
   int mapOffset = 0;

   while(!(keys & KEY_B))
   {
      scanKeys();

      keys = keysHeld();

      swiWaitForVBlank();

      if(keys & KEY_LEFT)
      {
         mapOffset = scroll_x / 8 - 1;
         layerOffset = (mapOffset & 63);

         if(layerOffset >= 32) layerOffset += 32 * 32 - 32;

         for(int iy = 0; iy < 24; iy++)
            map[layerOffset + (iy * 32)] = Layer512x256Map[(mapOffset & 63) + (iy * 64)] ;

         scroll_x--;
      }
      if(keys & KEY_RIGHT)		
      {
         mapOffset = scroll_x / 8 + 32;
         layerOffset = (mapOffset & 63);

         if(layerOffset >= 32) layerOffset += 32 * 32 - 32;

         for(int iy = 0; iy < 24; iy++)
            map[layerOffset + (iy * 32)] = Layer512x256Map[(mapOffset & 63) + (iy * 64)] ;

         scroll_x++;
      }


      bgSetScroll(bg, scroll_x, 0);
	  bgUpdate();
   }

}

void scrollHorizontalExRotation(void)
{
   videoSetMode(MODE_5_2D);
   vramSetBankA(VRAM_A_MAIN_BG);

   int scroll_x = 0;

   int bg = bgInit(3, BgType_ExRotation, BgSize_ER_512x512, 0,1);

   dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
   dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));

   u16* map = (u16*)bgGetMapPtr(bg);

   bgSetControlBits(bg, BG_WRAP_ON);

   for(int iy = 0; iy < 24; iy++)
      dmaCopy(&Layer512x256Map[iy * 64], &map[iy * 64], 32*2);

   int keys = 0;
   int offset = 0;

   while(!(keys & KEY_B))
   {
      scanKeys();

      keys = keysHeld();

      swiWaitForVBlank();

      if(keys & KEY_LEFT)
      {
         offset = scroll_x / 8 - 1;

         for(int iy = 0; iy < 24; iy++)
            map[(offset & 63) + (iy * 64)] = Layer512x256Map[(offset & 63) + (iy * 64)] ;

         scroll_x--;
      }
      if(keys & KEY_RIGHT)		
      {
         offset = scroll_x / 8 + 32;

         for(int iy = 0; iy < 24; iy++)
            map[(offset & 63) + (iy * 64)] = Layer512x256Map[(offset & 63) + (iy * 64)] ;

         scroll_x++;
      }

      bgSetScroll(bg, scroll_x, 0);
	  bgUpdate();
   }

}
//scrolls a 1024 by 1024 map on a 512x512 layer
void scroll4wayText(void)
{
   videoSetMode(MODE_0_2D);
   vramSetBankA(VRAM_A_MAIN_BG);

   int scroll_x = 0;
   int scroll_y = 0;

   const int tileWidth = 8; //width of a tile in pixels
   const int mapWidth = 1024 / 8; //width of the big map in tiles
   const int mapHeight = 1024 / 8; //heigh of the big map in tiles
   const int bgWidth = 256 / 8; //width of the hardware map in tiles
   const int bgHeight = 256 / 8; //height of the hardware map in tiles
   const int screenWidth = 256 / 8; //screen width in tiles
   const int screenHeight = 192 / 8; //screen height in tiles


   int bg = bgInit(3, BgType_Text8bpp, BgSize_T_512x256, 0,1);

   dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
   dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));

   u16* bgTileMap = (u16*)bgGetMapPtr(bg);
   u16* bgLeftHalf = bgTileMap;
   u16* bgRightHalf = bgTileMap + 32 * 32;

   for(int iy = 0; iy < screenHeight; iy++)
      dmaCopy(&Layer1024x1024Map[iy * mapWidth], &bgTileMap[iy * bgWidth], screenWidth * 2);

   int keys = 0;
   int offset_x = 0;
   int offset_y = 0;
   bool movingHorizontal = false;
   bool movingVertical = false;

   while(!(keys & KEY_B))
   {
      movingHorizontal = false;
      movingVertical = false;

      scanKeys();

      keys = keysHeld();

      swiWaitForVBlank();

      if(keys & KEY_LEFT)
      {
         offset_x = scroll_x / 8 - 1;
         scroll_x--;

         if(scroll_x < 0) 
            scroll_x = 0;
         else 
            movingHorizontal = true;
      }
      else if(keys & KEY_RIGHT)		
      {
         offset_x = scroll_x / 8 + screenWidth;
         scroll_x++;

         if(scroll_x >= (mapWidth - screenWidth) * tileWidth) 
            scroll_x = (mapWidth - screenWidth) * tileWidth - 1;
         else 
            movingHorizontal = true;
      }

      if(keys & KEY_UP)
      {
         offset_y = scroll_y / 8 - 1;
         scroll_y--;

         if(scroll_y < 0) 
            scroll_y = 0;
         else 
            movingVertical = true;

      }
      else if(keys & KEY_DOWN)		
      {
         offset_y = scroll_y / 8 + screenHeight;
         scroll_y++;

         if(scroll_y >= (mapHeight - screenHeight) * tileWidth) 
            scroll_y = (mapHeight - screenHeight) * tileWidth - 1;
         else 
            movingVertical = true;
      }

      if(movingHorizontal)
      {
         u16* bgTemp = ((offset_x & 63) >= bgWidth) ? bgRightHalf : bgLeftHalf;

         for(int iy = scroll_y / 8 - 1 ; iy < scroll_y / 8 + screenHeight + 1; iy++)
         {	
            bgTemp[(offset_x & (bgWidth - 1)) + (iy & (bgHeight - 1)) * 32] = 
               Layer1024x1024Map[offset_x + iy * mapWidth];	
         }
      }
      if(movingVertical)
      {	
         for(int ix = scroll_x / 8 - 1 ; ix < scroll_x / 8 + screenWidth + 1; ix++)
         {
            u16* bgTemp = ((ix & 63) >= bgWidth) ? bgRightHalf : bgLeftHalf;

             bgTemp[(ix & (bgWidth - 1)) + (offset_y & (bgHeight - 1))* 32] = 
                 Layer1024x1024Map[ix + offset_y * mapWidth];	
         }
      }

      bgSetScroll(bg, scroll_x, scroll_y);
	  bgUpdate();
   }
}

void scroll4wayExRotation(void)
{
   videoSetMode(MODE_5_2D);
   vramSetBankA(VRAM_A_MAIN_BG);

   int scroll_x = 0;
   int scroll_y = 0;

   const int tileWidth = 8;
   const int mapWidth = 1024 / 8;
   const int mapHeight = 1024 / 8;
   const int bgWidth = 512 / 8;
   const int bgHeight = 512 / 8;
   const int screenWidth = 256 / 8;
   const int screenHeight = 192 / 8;


   int bg = bgInit(3, BgType_ExRotation, BgSize_ER_512x512, 0,1);

   dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
   dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));

   u16* bgTileMap = (u16*)bgGetMapPtr(bg);

   bgSetControlBits(bg, BG_WRAP_ON);

   for(int iy = 0; iy < screenHeight; iy++)
      dmaCopy(&Layer1024x1024Map[iy * mapWidth], &bgTileMap[iy * bgWidth], screenWidth * 2);

   int keys = 0;
   int offset_x = 0;
   int offset_y = 0;
   bool movingHorizontal = false;
   bool movingVertical = false;

   while(!(keys & KEY_B))
   {
      movingHorizontal = false;
      movingVertical = false;

      scanKeys();

      keys = keysHeld();

      swiWaitForVBlank();

      if(keys & KEY_LEFT)
      {
         offset_x = scroll_x / 8 - 1;
         scroll_x--;

         if(scroll_x < 0) 
            scroll_x = 0;
         else 
            movingHorizontal = true;
      }
      else if(keys & KEY_RIGHT)		
      {
         offset_x = scroll_x / 8 + screenWidth;
         scroll_x++;

         if(scroll_x >= (mapWidth - screenWidth) * tileWidth) 
            scroll_x = (mapWidth - screenWidth) * tileWidth - 1;
         else 
            movingHorizontal = true;
      }

      if(keys & KEY_UP)
      {
         offset_y = scroll_y / 8 - 1;
         scroll_y--;

         if(scroll_y < 0) 
            scroll_y = 0;
         else 
            movingVertical = true;

      }
      else if(keys & KEY_DOWN)		
      {
         offset_y = scroll_y / 8 + screenHeight;
         scroll_y++;

         if(scroll_y >= (mapHeight - screenHeight) * tileWidth) 
            scroll_y = (mapHeight - screenHeight) * tileWidth - 1;
         else 
            movingVertical = true;
      }

      if(movingHorizontal)
      {
         for(int iy = scroll_y / 8 - 1 ; iy < scroll_y / 8 + screenHeight + 1; iy++)
            bgTileMap[(offset_x & (bgWidth - 1)) + (iy & (bgHeight - 1)) * bgWidth] = 
               Layer1024x1024Map[offset_x  + iy * mapWidth];	
      }
      if(movingVertical)
      {	
         for(int ix = scroll_x / 8 - 1 ; ix < scroll_x / 8 + screenWidth + 1; ix++)
            bgTileMap[(ix & (bgWidth - 1)) + (offset_y & (bgHeight - 1))* bgWidth] = 
               Layer1024x1024Map[ix + offset_y * mapWidth];	
      }

      bgSetScroll(bg, scroll_x, scroll_y);
	  bgUpdate();
   }
}
