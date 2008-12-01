#include <nds.h>

#include <RotBackgrounds.h>
#include <TextBackgrounds.h>
#include <Multilayer.h>

#include <stdio.h>
#include <stdlib.h>

void scroll(int id, int width, int height);

void Text256x256(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(Layer256x256Map, bgGetMapPtr(bg),  Layer256x256MapLen);
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	
	scroll(bg, 256, 256);
}

void Text256x512(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x512, 0,1);
	
	dmaCopy(TextBackgroundsTiles, bgGetGfxPtr(bg), sizeof(TextBackgroundsTiles));
	dmaCopy(Layer256x512Map, bgGetMapPtr(bg),  Layer256x512MapLen);
	dmaCopy(TextBackgroundsPal, BG_PALETTE, sizeof(TextBackgroundsPal));
	
	scroll(bg, 256, 512);
}


void Text512x256(void)
{
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x256, 0,1);
	
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
	
	int bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x512, 0,1);
	
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
	
	int bg = bgInit(3, BgType_Rotation, BgSize_R_256x256, 0,2);
	
	dmaCopy(RotBackgroundsTiles, bgGetGfxPtr(bg), sizeof(RotBackgroundsTiles));
	dmaCopy(RotBackgroundsPal, BG_PALETTE, sizeof(RotBackgroundsPal));
	dmaCopy(Layer256x256rMap, bgGetMapPtr(bg),  Layer256x256rMapLen);
	
	scroll(bg, 256, 256);
}


void Rot512x512(void)
{
	videoSetMode(MODE_2_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Rotation, BgSize_R_512x512, 0,2);
	
	dmaCopy(RotBackgroundsTiles, bgGetGfxPtr(bg), sizeof(RotBackgroundsTiles));
	dmaCopy(RotBackgroundsPal, BG_PALETTE, sizeof(RotBackgroundsPal));
	dmaCopy(Layer512x512rMap, bgGetMapPtr(bg),  Layer512x512rMapLen);

	scroll(bg, 512, 512);
}

void Rot1024x1024(void)
{
	videoSetMode(MODE_2_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	int bg = bgInit(3, BgType_Rotation, BgSize_R_1024x1024, 0,3);
	
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
