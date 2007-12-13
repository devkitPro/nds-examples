#include <nds.h>

#include <RotBackgrounds.h>
#include <TextBackgrounds.h>
#include <Multilayer.h>

#include <stdio.h>
#include <stdlib.h>


u8 tiles[] =
{
//Tile 0: transperent tile
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
//Tile 1: solid tile using color index 1
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
//Tile 2: solid tile using color index 2	
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
//Tile 3: smily face tile	
	0,0,1,1,1,1,0,0,
	0,1,1,1,1,1,1,0,
	1,1,2,1,1,2,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,2,1,1,1,1,2,1,
	0,1,2,2,2,2,1,0,
	0,0,1,1,1,1,0,0,

	
};


u16 map32x32[] = 
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	
};

u16 palette[] = 
{
	RGB15(0,0,0),
	RGB15(31,31,0),
	RGB15(0,31,0)
};

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
		
		consoleClear();
		iprintf("Scroll x: %d Scroll y: %d\n", sx, sy);
		iprintf("Press 'B' to exit");
	}
}

void HandMadeTiles(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text, BgSize_T_256x256, 0,1);
	
	dmaCopy(tiles, bgGetGfxPtr(bg), sizeof(tiles));
	dmaCopy(map32x32, bgGetMapPtr(bg),  sizeof(map32x32));
	dmaCopy(palette, BG_PALETTE, sizeof(palette));
	
	scroll(bg, 256, 256);
}
void Text256x256(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text, BgSize_T_256x256, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(Layer256x256Map, bgGetMapPtr(bg),  Layer256x256MapLen);
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	
	scroll(bg, 256, 256);
}

void Text256x512(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text, BgSize_T_256x512, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(Layer256x512Map, bgGetMapPtr(bg),  Layer256x512MapLen);
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	
	scroll(bg, 256, 512);
}


void Text512x256(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text, BgSize_T_512x256, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));

	u16* map = (u16*)bgGetMapPtr(bg);
	
	for(int iy = 0; iy < 32; iy++)
	{
		//first the left half
		dmaCopy(Layer512x256Map + iy * 64, map + iy * 32,  32 * 2);
		
		//then the right half
		dmaCopy(Layer512x256Map + iy * 64 + 32, map + (32 * 32) + iy * 32,  32 * 2);
	}
	
	scroll(bg, 512, 256);
}

void Text512x512(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text, BgSize_T_512x512, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));

	u16* map = (u16*)bgGetMapPtr(bg);
	
	//draw top half
	for(int iy = 0; iy < 32; iy++)
	{
		//first the left half
		dmaCopy(Layer512x512Map + iy * 64, map + iy * 32,  32 * 2);
		
		//then the right half
		dmaCopy(Layer512x512Map + iy * 64 + 32, map + (32 * 32) + iy * 32,  32 * 2);
	}
	
	map += 32 * 32 * 2;
	
	//draw bottom half
	for(int iy = 0; iy < 32; iy++)
	{
		//copy one line at a time
		//first the left half
		dmaCopy(Layer512x512Map + (iy + 32) * 64, map + iy * 32,  32 * 2);
		
		//then the right half
		dmaCopy(Layer512x512Map + (iy + 32) * 64 + 32, map + (32 * 32) + iy * 32,  32 * 2);
	}
	
	scroll(bg, 512, 512);
}

void ExRot128x128(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_ExRotation, BgSize_ER_128x128, 0, 1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(Layer128x128Map, bgGetMapPtr(bg),  Layer128x128MapLen);
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	
	scroll(bg, 128, 128);
}

void ExRot256x256(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_ExRotation, BgSize_ER_256x256, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(Layer256x256Map, bgGetMapPtr(bg),  Layer256x256MapLen);
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	
	scroll(bg, 256, 256);
}


void ExRot512x512(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_ExRotation, BgSize_ER_512x512, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	dmaCopy(Layer512x512Map, bgGetMapPtr(bg),  Layer512x512MapLen);

	scroll(bg, 512, 512);
}

void ExRot1024x1024(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_ExRotation, BgSize_ER_1024x1024, 0,2);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	dmaCopy(Layer1024x1024Map, bgGetMapPtr(bg),  Layer1024x1024MapLen);

	scroll(bg, 1024, 1024);
}

void Rot128x128(void)
{
	videoSetMode(MODE_2_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Rotation, BgSize_R_128x128, 0, 1);
	
	dmaCopy(RotBackgroundsTiles, bgGetGfxPtr(bg), sizeof(RotBackgroundsTiles));
	dmaCopy(RotBackgroundsPal, BG_PALETTE, sizeof(RotBackgroundsPal));
	dmaCopy(Layer128x128rMap, bgGetMapPtr(bg),  Layer128x128rMapLen);
	
	scroll(bg, 128, 128);
}

void Rot256x256(void)
{
	videoSetMode(MODE_2_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Rotation, BgSize_R_256x256, 0,1);
	
	dmaCopy(RotBackgroundsTiles, bgGetGfxPtr(bg), sizeof(RotBackgroundsTiles));
	dmaCopy(RotBackgroundsPal, BG_PALETTE, sizeof(RotBackgroundsPal));
	dmaCopy(Layer256x256rMap, bgGetMapPtr(bg),  Layer256x256rMapLen);
	
	scroll(bg, 256, 256);
}


void Rot512x512(void)
{
	videoSetMode(MODE_2_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Rotation, BgSize_R_512x512, 0,1);
	
	dmaCopy(RotBackgroundsTiles, bgGetGfxPtr(bg), sizeof(RotBackgroundsTiles));
	dmaCopy(RotBackgroundsPal, BG_PALETTE, sizeof(RotBackgroundsPal));
	dmaCopy(Layer512x512rMap, bgGetMapPtr(bg),  Layer512x512rMapLen);

	scroll(bg, 512, 512);
}

void Rot1024x1024(void)
{
	videoSetMode(MODE_2_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Rotation, BgSize_R_1024x1024, 0,2);
	
	dmaCopy(RotBackgroundsTiles, bgGetGfxPtr(bg), sizeof(RotBackgroundsTiles));
	dmaCopy(RotBackgroundsPal, BG_PALETTE, sizeof(RotBackgroundsPal));
	dmaCopy(Layer1024x1024rMap, bgGetMapPtr(bg),  Layer1024x1024rMapLen);

	scroll(bg, 1024, 1024);
}



void Bmp8_128x128(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Bmp8, BgSize_B8_128x128,0,0);
	
	for(int i = 0; i < 256; i++)
		BG_PALETTE[i] = rand();
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 128; iy++)
		for(int ix = 0; ix < 128 / 2; ix++)
		{
			buffer[ix + iy * 64] = rand();
		}
		
	scroll(bg, 128, 128);
}

void Bmp8_256x256(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256,0,0);
	
	for(int i = 0; i < 256; i++)
		BG_PALETTE[i] = rand();
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 256; iy++)
		for(int ix = 0; ix < 256 / 2; ix++)
		{
			buffer[ix + iy * 128] = rand();
		}
		
	scroll(bg, 256, 256);

}
void Bmp8_512x256(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Bmp8, BgSize_B8_512x256,0,0);
	
	for(int i = 0; i < 256; i++)
		BG_PALETTE[i] = rand();
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 256; iy++)
		for(int ix = 0; ix < 512 / 2; ix++)
		{
			buffer[ix + iy * 256] = rand();
		}
		
	scroll(bg, 512, 256);
}

void Bmp8_512x512(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
	
	int bg = bgInit(3, BgType_Bmp8, BgSize_B8_512x512,0,0);
	
	for(int i = 0; i < 256; i++)
		BG_PALETTE[i] = rand();
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 512; iy++)
		for(int ix = 0; ix < 512 / 2; ix++)
		{
			buffer[ix + iy * 256] = rand();
		}
		
	scroll(bg, 512, 512);
}
void Bmp8_512x1024(void)
{
	videoSetMode(MODE_6_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
	vramSetBankC(VRAM_C_MAIN_BG);
	vramSetBankD(VRAM_D_MAIN_BG);
	
	int bg = bgInit(2, BgType_Bmp8, BgSize_B8_512x1024,0,0);
	
	for(int i = 0; i < 256; i++)
		BG_PALETTE[i] = rand();
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 1024; iy++)
		for(int ix = 0; ix < 512 / 2; ix++)
		{
			buffer[ix + iy * 256] = rand();
		}
		
	scroll(bg, 512, 1024);
}
void Bmp8_1024x512(void)
{
	videoSetMode(MODE_6_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
	vramSetBankC(VRAM_C_MAIN_BG);
	vramSetBankD(VRAM_D_MAIN_BG);
	
	int bg = bgInit(2, BgType_Bmp8, BgSize_B8_1024x512,0,0);
	
	for(int i = 0; i < 256; i++)
		BG_PALETTE[i] = rand();
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 512; iy++)
		for(int ix = 0; ix < 1024 / 2; ix++)
		{
			buffer[ix + iy * 512] = rand();
		}
		
	scroll(bg, 1024, 512);
}

void Bmp16_128x128(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(2, BgType_Bmp16, BgSize_B16_128x128,0,0);
	
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 128; iy++)
		for(int ix = 0; ix < 128; ix++)
		{
			buffer[ix + iy * 128] = rand();
		}
		
	scroll(bg, 128, 128);
}
void Bmp16_256x256(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(2, BgType_Bmp16, BgSize_B16_256x256,0,0);
	
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 256; iy++)
		for(int ix = 0; ix < 256; ix++)
		{
			buffer[ix + iy * 256] = rand();
		}
		
	scroll(bg, 256, 256);
}
void Bmp16_512x256(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
	
	int bg = bgInit(2, BgType_Bmp16, BgSize_B16_512x256,0,0);
	
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 256; iy++)
		for(int ix = 0; ix < 512; ix++)
		{
			buffer[ix + iy * 512] = rand();
		}
		
	scroll(bg, 512, 256);
}
void Bmp16_512x512(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
	vramSetBankC(VRAM_C_MAIN_BG);
	vramSetBankD(VRAM_D_MAIN_BG);

	int bg = bgInit(2, BgType_Bmp16, BgSize_B16_512x512,0,0);
	
	
	u16* buffer = (u16*)bgGetGfxPtr(bg);
	
	for(int iy = 0; iy < 512; iy++)
		for(int ix = 0; ix < 512; ix++)
		{
			buffer[ix + iy * 512] = rand();
		}
		
	scroll(bg, 512, 512);
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
	
	int bg = bgInit(0, BgType_Text, BgSize_T_256x512, 0,1);
	
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
		
		BG0_X0 = sx;
		BG0_Y0 = sy;
		
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
		
		BG3_CX = sx << 8;
		BG3_CY = sy << 8;
		
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
	
	int bg = bgInit(0, BgType_Text, BgSize_T_256x256, 0,1);
	
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
	}

}
void scrollHorizontalText(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int scroll_x = 0;
	
	int bg = bgInit(0, BgType_Text, BgSize_T_512x256, 0,1);
	
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
	}

}
void scroll4wayText(void)
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
					Layer1024x1024Map[(offset_x & (mapWidth - 1)) + (iy & (mapHeight - 1)) * mapWidth];	
		}
		if(movingVertical)
		{	
			for(int ix = scroll_x / 8 - 1 ; ix < scroll_x / 8 + screenWidth + 1; ix++)
				bgTileMap[(ix & (bgWidth - 1)) + (offset_y & (bgHeight - 1))* bgWidth] = 
					Layer1024x1024Map[(ix & (mapWidth - 1)) + (offset_y & (mapHeight - 1))* mapWidth];	
		}
	
		bgSetScroll(bg, scroll_x, scroll_y);
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
					Layer1024x1024Map[(offset_x & (mapWidth - 1)) + (iy & (mapHeight - 1)) * mapWidth];	
		}
		if(movingVertical)
		{	
			for(int ix = scroll_x / 8 - 1 ; ix < scroll_x / 8 + screenWidth + 1; ix++)
				bgTileMap[(ix & (bgWidth - 1)) + (offset_y & (bgHeight - 1))* bgWidth] = 
					Layer1024x1024Map[(ix & (mapWidth - 1)) + (offset_y & (mapHeight - 1))* mapWidth];	
		}
	
		bgSetScroll(bg, scroll_x, scroll_y);
	}
}

void advMosaic(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text, BgSize_T_256x256, 0,1);
	
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
		iprintf("Press B to exit.n");
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

		if(keys & KEY_L) angle--;
		if(keys & KEY_R) angle++;

		if(center_x > 256) center_x = 256;
		if(center_x < 0) center_x = 0;
		
		if(center_y > 192) center_y = 192;
		if(center_y < 0) center_y = 0;

		
		swiWaitForVBlank();
		
		bgSetRotate(bg, angle);
		bgSetScroll(bg, center_x, center_y);
		bgSetCenter(bg, center_x, center_y);
		
		consoleClear();
		iprintf("Press B to exit.n");
		iprintf("Angle: %d \ncenter X: %d  center Y: %d", angle, center_x, center_y);
		
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
		
		consoleClear();
		iprintf("Press B to exit.n");
		iprintf("scale X: %d  scale Y: %d", scale_x, scale_y);
		
	}
}
void advExtendedPalette(void){}
void advMultipleLayers(void)
{
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	//initialize the backgrounds
	int bg1 = bgInit(0, BgType_Text, BgSize_ER_256x256, 0,1);
	int bg2 = bgInit(1, BgType_Text, BgSize_ER_256x256, 1,1);
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


struct Demo
{
	fp go;
	const char* name;
	const char* description;
};

struct Category
{
	const char* name;
	Demo *demos;
	int count;
};

struct Demo basicExamples[] =
{ 
	{HandMadeTiles, "Handmade Text 256x256", "Displays a handmade 256 x 256 Text background"},
	
	{Text256x256, "Text 256x256", "Displays a 256 x 256 Text background"},
	{Text256x512, "Text 256x512", "Displays a 256 x 512 Text background"},
	{Text512x256, "Text 512x256", "Displays a 512 x 256 Text background"},
	{Text512x512, "Text 512x512", "Displays a 512 x 512 Text background"},
	
	{ExRot128x128, "Extended Rotation 128x128", "Displays a 128 x 128 Extended Rotation background"},
	{ExRot256x256, "Extended Rotation 256x256", "Displays a 256 x 256 Extended Rotation background"},
	{ExRot512x512, "Extended Rotation 512x512", "Displays a 512 x 512 Extended Rotation background"},
	{ExRot1024x1024, "Extended Rotation 1024x1024", "Displays a 1024 x 1024 Extended Rotation background"},
	
	{Rot128x128, "Rotation 128x128", "Displays a 256 x 256 Rotation background"},
	{Rot256x256, "Rotation 256x256", "Displays a 256 x 256 Rotation background"},
	{Rot512x512, "Rotation 512x512", "Displays a 512 x 512 Rotation background"},
	{Rot1024x1024, "Rotation 1024x1024", "Displays a 1024 x 1024 Rotation background"},

	
};

struct Demo bitmapExamples[] = 
{
	{Bmp8_128x128, "256 color 128x128", "Displays a 128 x 128 Bitmap background"},
	{Bmp8_256x256, "256 color 256x256", "Displays a 256 x 256 Bitmap background"},
	{Bmp8_512x256, "256 color  512x256", "Displays a 512 x 256 Bitmap background"},
	{Bmp8_512x512, "256 color  512x512", "Displays a 512 x 512 Bitmap background"},
	{Bmp8_512x1024, "256 color  512x1024", "Displays a 512 x 1024 Bitmap background"},
	{Bmp8_1024x512, "256 color  1024x512", "Displays a 1024 x 512 Bitmap background"},
	
	{Bmp16_128x128, "16-bit color 128x128", "Displays a 128 x 128 Bitmap background"},
	{Bmp16_256x256, "16-bit color  256x256", "Displays a 256 x 256 Bitmap background"},
	{Bmp16_512x256, "16-bit color  512x256", "Displays a 512 x 256 Bitmap background"},
	{Bmp16_512x512, "16-bit color  512x512", "Displays a 512 x 512 Bitmap background"},
	

};

struct Demo scrollingExamples[] =
{
	{scrollText, "Text Backgrounds", "Hardware Scrolling of a Text background"},
	{scrollRotation, "Rot Backgrounds", "Hardware Scrolling of a Rotation background"},
	{scrollVertical, "Vertical Scrolling", "Scrolling a large map vertically"},
	{scrollHorizontalText, "Horizontal Scrolling (Text)", "Scrolling a large map horzontally on a text background"},
	{scrollHorizontalExRotation, "Horizontal Scrolling (ExRot)", "Scrolling a large map horzontally on an extended rotation background"},
	{scroll4wayText, "4 Way Scrolling (Text)", "Scrolling a large map 4 ways on a text background"},
	{scroll4wayExRotation, "4 Way Scrolling (Rotation)", "Scrolling a large map 4 ways on a ex rotation background"},

};

struct Demo advancedExamples[] =
{
	{advMosaic, "Mosaic", "A demo of the Mosaic scaling"},
	{advRotating, "Rotation", ""},
	{advScaling, "Scaling", ""},
	{advExtendedPalette, "Extended Palette", ""},
	{advMultipleLayers, "Multiple Text Layers", ""},

};

struct Category categories[] = 
{
	{"Basic", basicExamples, sizeof(basicExamples) / sizeof(Demo)},
	{"Bitmap", bitmapExamples, sizeof(bitmapExamples) / sizeof(Demo)},
	{"Scrolling", scrollingExamples, sizeof(scrollingExamples) / sizeof(Demo)},
	{"Advanced", advancedExamples, sizeof(advancedExamples) / sizeof(Demo)}

};

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int keys;
	

	
	while(1)
	{
		int selectedCategory = 0;
		int selectedDemo = 0;
		
		bool selected = false;
		
		int catCount = sizeof(categories) / sizeof(Category);
		int demoCount = 0;
		
		consoleDemoInit();
		irqInit();
		irqEnable(IRQ_VBLANK);
		
		while(!selected)
		{
			scanKeys();
			
			
			keys = keysDown();
			
			if(keys & KEY_UP) selectedCategory--;
			if(keys & KEY_DOWN) selectedCategory++;
			if(keys & KEY_A) selected = true;
			
			if(selectedCategory < 0) selectedCategory = catCount - 1;
			if(selectedCategory >= catCount) selectedCategory = 0;
			
			swiWaitForVBlank();
			consoleClear();
			for(int ci = 0; ci < catCount; ci++)
			{
				iprintf("%c%d: %s\n", ci == selectedCategory ? '*' : ' ', ci + 1, categories[ci].name); 		
			}	
		}
		
		selected = false;
		
		demoCount = categories[selectedCategory].count;
		
		while(!selected)
		{
			scanKeys();
			
			
			keys = keysDown();
			
			if(keys & KEY_UP) selectedDemo--;
			if(keys & KEY_DOWN) selectedDemo++;
			if(keys & KEY_A) selected = true;
			if(keys & KEY_B) break;
			
			if(selectedDemo < 0) selectedDemo = demoCount - 1;
			if(selectedDemo >= demoCount) selectedDemo = 0;
			
			swiWaitForVBlank();
			consoleClear();
			for(int di = 0; di < demoCount; di++)
			{
				iprintf("%c%d: %s\n", di == selectedDemo ? '*' : ' ', di + 1, categories[selectedCategory].demos[di].name); 		
			}	
		}
		
		if(selected)
		{
			consoleClear();
			iprintf("Use arrow keys to scroll\nPress 'B' to exit");
			categories[selectedCategory].demos[selectedDemo].go();
		}
	}
	
}


