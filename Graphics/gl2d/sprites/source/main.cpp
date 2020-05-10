/******************************************************************************
*******************************************************************************
	Easy GL2D 
	Sprites example
	Shows a some sprite capabilities of easy GL2D
	
	Relminator (Richard Eric M. Lope BSN RN)
	Http://Rel.Phatcode.Net
	
	Thanks to:
	Anya Therese B. Lope for letting me use her picture
	Adigun A. Polack for the enemies sprites
	Patater (Jaeden Amero) for the shuttle sprites
	capcom for Zero's image
	Marc Russel for the main screen font
	Libnds examples for the bottom screen fonts

*******************************************************************************
******************************************************************************/ 
 
#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <gl2d.h>
#include "cearn_atan.h"

// GRIT auto-generated  files
#include "enemies.h"
#include "zero.h"
#include "tiles.h"
#include "shuttle.h"
#include "anya.h"

#include "font.h"
#include "fontbubble.h"


// Texture Packer auto-generated UV coords

#include "uvcoord_enemies.h"
#include "uvcoord_zero.h"


// Declare our BG drawing routine
void DrawBG( glImage *images );



const int BRAD_PI = 1 << 14;

// This imagesets would use our texture packer generated coords so it's kinda
// safe and easy to use 
// ENEMIES_NUM_IMAGES is a value from "uvcoord_crono.h"
// ZERO_NUM_IMAGES is a value from "uvcoord_zero.h"
glImage  Enemies[ENEMIES_NUM_IMAGES];	// spriteset
glImage  Zero[ZERO_NUM_IMAGES];			// spriteset

// This tileset won't make use of our texture packer generated coords.
// Messy, manual and prone to errors.
// BMP is 256x256 and tiles are 16x16 so.. (256/16) * (256 /16) = 16 * 16
glImage  Tiles[(256/16) * (256/16)];  

// These sprites are single texture only so no need to
// do anything special
glImage  Shuttle[1];					// single image	
glImage  Anya[1];


int main() 
{	

	// Console fonts for both screen
	PrintConsole topScreen;
    PrintConsole bottomScreen;
	
	//set mode 5, enable BG0 and set it to 3D
	videoSetMode( MODE_5_3D );
	videoSetModeSub( MODE_0_2D  );
	
	
	// initialize gl2d
	glScreen2D();
	
	// Set up enough texture memory for our textures
	// Bank A is just 128kb and we are using 194 kb of
	// sprites
	vramSetBankA( VRAM_A_TEXTURE );
	vramSetBankB( VRAM_B_TEXTURE );

	vramSetBankF(VRAM_F_TEX_PALETTE);  // Allocate VRAM bank for all the palettes
	
	vramSetBankE(VRAM_E_MAIN_BG);	 // Main bg text/8bit bg. Bank E size == 64kb, exactly enough for 8bit * 256 * 192 + text layer
	
	// Load our custom font for the top screen
	consoleInit(&topScreen,1, BgType_Text4bpp, BgSize_T_256x256, 31,0, true, false);
	//put bg 0 at a lower priority than the text background
	bgSetPriority(0, 1);

	// Bottom screeen
	vramSetBankI( VRAM_I_SUB_BG_0x06208000 );
	consoleInit( &bottomScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 20, 0, false, false );
	
	
	// Bottom Screen font
	ConsoleFont font;

	font.gfx = (u16*)fontTiles;
	font.pal = (u16*)fontPal;
	font.numChars = 95;
	font.numColors =  fontPalLen / 2;
	font.bpp = 4;
	font.asciiOffset = 32;
	font.convertSingleColor = false;
	
	
	// Top Screen font
	ConsoleFont fontbubble;

	fontbubble.gfx = (u16*)fontbubbleTiles;
	fontbubble.pal = (u16*)fontbubblePal;
	fontbubble.numChars = 64;
	fontbubble.numColors =  fontbubblePalLen / 2;
	fontbubble.bpp = 4;
	fontbubble.asciiOffset = 32;
	fontbubble.convertSingleColor = false;
	
	consoleSetFont(&bottomScreen, &font);
	consoleSetFont(&topScreen, &fontbubble);
	
	// Make an all-white palette for stencil effects
	unsigned short *hitPal = new unsigned short[256];
	int i;
	for( i = 0; i < 256; i++ )
	{
		hitPal[i] = (0xFF << 8 ) | 0xFF;
	}
	
	
	// Our special palettes
	int PaletteID;			// the all-white pal
	int OriginalPaletteID;	// Original palette
	
	// Generate the texture and load the all-white palette
	glGenTextures(1, &PaletteID);
	glBindTexture(0, PaletteID);
	glColorTableEXT(0,0,256,0,0,hitPal);
	
	// Generate another palette for our original image palette
	glGenTextures(1, &OriginalPaletteID);
	glBindTexture(0, OriginalPaletteID);
	glColorTableEXT(0,0,256,0,0,enemiesPal);
	
	// free some memory
	delete[] hitPal;
	
	// Load our Enemies texture
	// We used glLoadSpriteSet since the texture was made
	// with my texture packer.
	int EnemiesTextureID = 
		glLoadSpriteSet( Enemies,				// pointer to glImage array
						 ENEMIES_NUM_IMAGES, 	// Texture packer auto-generated #define
						 enemies_texcoords,		// Texture packer auto-generated array
						 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
						 TEXTURE_SIZE_256,		// sizeX for glTexImage2D() in videoGL.h
						 TEXTURE_SIZE_256,		// sizeY for glTexImage2D() in videoGL.h
						 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
						 256,					// Length of the palette to use (256 colors)
						 (u16*)enemiesPal,		// Load our 256 color enemies palette
						 (u8*)enemiesBitmap	 	// image data generated by GRIT
					   );
	
	// Load our Zero texture
	// We used glLoadSpriteSet since the texture was made
	// with my texture packer.
	// No need for another palette since enemies and zero
	// share the same palette.
	int ZeroTextureID = 
		glLoadSpriteSet( Zero,					// pointer to glImage array
						 ZERO_NUM_IMAGES, 		// Texture packer auto-generated #define
						 zero_texcoords,		// Texture packer auto-generated array
						 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
						 TEXTURE_SIZE_128,		// sizeX for glTexImage2D() in videoGL.h
						 TEXTURE_SIZE_256,		// sizeY for glTexImage2D() in videoGL.h
						 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
						 256,					// Length of the palette to use (256 colors)
						 (u16*)enemiesPal,		// Zero and Enemies share the same palette
						 (u8*)zeroBitmap	 	// image data generated by GRIT
					   );
	
	
	// Our texture handle for our tiles
	// I used glLoadTileSet since the texture 
	// is just a bunch of 16x16 tiles in a 256x256
	// tileset so we don't need a texture packer for this.
	int TilesTextureID = 
		glLoadTileSet( Tiles,				// pointer to glImage array
					   16,					// sprite width
					   16,					// sprite height
					   256,					// bitmap width
					   256,					// bitmap height
					   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
					   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
					   TEXTURE_SIZE_256,	// sizeY for glTexImage2D() in videoGL.h
					   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					   256,					// Length of the palette to use (256 colors)
					   (u16*)tilesPal,		// Load our 256 color tiles palette
					   (u8*)tilesBitmap		// image data generated by GRIT
					 );
	
	
	
	// Shuttle
	// Since the shuttle is just a single 64x64 image,
	// We use glLoadTileSet() giving the right dimensions.
	int ShuttleTextureID = 
		glLoadTileSet( Shuttle,			// pointer to glImage array
					   64,				// sprite width
					   64,				// sprite height
					   64,				// bitmap image width
					   64,				// bitmap image height
					   GL_RGB16,		// texture type for glTexImage2D() in videoGL.h
					   TEXTURE_SIZE_64,	// sizeX for glTexImage2D() in videoGL.h
					   TEXTURE_SIZE_64,	// sizeY for glTexImage2D() in videoGL.h
					   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					   16,					// Length of the palette to use (16 colors)
					   (u16*)shuttlePal,		// Load our 256 color tiles palette
					   (u8*)shuttleBitmap // image data generated by GRIT
					 );

	

	// Anya
	// This is a 16 bit image
	int AnyaTextureID = 
		glLoadTileSet( Anya,
					   128,
					   128,
					   128,
					   128,
					   GL_RGB,
					   TEXTURE_SIZE_128,
					   TEXTURE_SIZE_128,
					   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF,
					   0,		// Just use 0 if palette is not in use
					   0,		// Just use 0 if palette is not in use
					   (u8*)anyaBitmap
					 );

	// Print some console stuff

	// Top
	consoleSelect(&topScreen);
    iprintf("\n\n\n\n\tWOOT!\n");
    iprintf("\tTOPSCREEN 3D+TEXT\n");
    
	
	// Bottom
    consoleSelect(&bottomScreen);
    iprintf("\x1b[1;1HEasy GL2D Sprites Demo");
	iprintf("\x1b[2;1HRelminator");
	iprintf("\x1b[4;1HHttp://Rel.Phatcode.Net");
	
	iprintf("\x1b[6;1HA demo showing some sprite");
	iprintf("\x1b[7;1Hcapabilities of Easy GL2D");
	
	iprintf("\x1b[ 9;1HSprites by:");
	iprintf("\x1b[10;1HAdigun A. Polack, Patater,");
	iprintf("\x1b[11;1HCapcom and Anya Therese Lope");
	
	iprintf("\x1b[13;1HTextureIDs = %i, %i, %i, %i, %i", 
			EnemiesTextureID,
			ZeroTextureID,
			TilesTextureID,
			ShuttleTextureID,
			AnyaTextureID
		   );
	
	
	// calculate the amount of 
	// memory uploaded to VRAM in KB
	int TextureSize = enemiesBitmapLen +
					  zeroBitmapLen +
					  tilesBitmapLen +
					  shuttleBitmapLen +
					  anyaBitmapLen;
					  
					  
	iprintf("\x1b[15;1HTotal Texture size= %i kb", TextureSize / 1024);
	
	
	iprintf("\x1b[17;1HEnemies use a 256 color pal");
	iprintf("\x1b[18;1HZero uses a 256 color pal");
	iprintf("\x1b[19;1HTiles use a 256 color pal");
	iprintf("\x1b[20;1HShuttle uses a 16 color pal");
	iprintf("\x1b[21;1HAnya is a 16 bit image");
	
	
	// some variables for our demo
	int Frame = 0;				// just the standard frame counter
	int PhoenixFrame = 0;		// animation frame for our firebird
	int BeeFrame = 0;			// animation frame for the bee
	int ZeroFrame = 0;			// animation frame for zero
	int Rotation = 0;			// rotation value of the rotating sprites
	
	
	while( 1 ) 
	{
	
		Frame++;
		
		Rotation = Frame * 240;		// speed up our rotation
		
		// animate some of our animated sprites
		// every 8th frame
		if ( (Frame & 7) == 0 )
		{
			BeeFrame = (BeeFrame + 1) & 1;
			PhoenixFrame++;
			if (PhoenixFrame > 2) PhoenixFrame = 0;
			
		}

		// Faster zero animation
		if ( (Frame & 3) == 0 )
		{

			ZeroFrame++;
			if (ZeroFrame > 9) ZeroFrame = 0;
		}
		
		
		// calculate positions for our rotating sprites
		int x = 128 + ((cosLerp(Frame)+sinLerp(BRAD_PI+Rotation) * 100) >> 12);
		int y = 96 + ((cosLerp(Frame)+cosLerp(-Rotation) * 80) >> 12);
			
		
		
		// Start 2D mode
		glBegin2D();

			// Set our palette to our tiles (256 colors)
			// and draw our background
			DrawBG( Tiles );
			
			// Make the Anya's rotoscaled sprite translucent just for kicks
			// Use glSpriteRotateScaleXY() for some effect
			// Give it a polygon ID so that transluceny would work
			glPolyFmt(POLY_ALPHA(20) | POLY_CULL_NONE | POLY_ID(1));
			glSpriteRotateScaleXY( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, Frame*140, sinLerp(Frame*120) * 3, sinLerp(Frame*210) * 2,  GL_FLIP_NONE, Anya );
			
			
			// Draw our enemies
			// draw some rotated and/or animated sprites
			// Give  the other sprites different polygon IDs
			// so that translucency works
			glPolyFmt(POLY_ALPHA(20) | POLY_CULL_NONE | POLY_ID(2));
			glSpriteRotate(    x,     y,      Rotation,          GL_FLIP_NONE, &Enemies[30+BeeFrame]);
			glSpriteRotate(255-x, 191-y,  Rotation * 4,             GL_FLIP_H, &Enemies[84]);
			glSpriteRotate(255-x,     y,     -Rotation,             GL_FLIP_V, &Enemies[32]);
			glSpriteRotate(    x, 191-y, -Rotation * 3, GL_FLIP_H | GL_FLIP_V, &Enemies[81]);
			
			
			// Some phoenix enemies on the right
			// Note the flipmodes 
			// Also shows how we can draw in "color mode" and shadow mode
			glPolyFmt(POLY_ALPHA(20) | POLY_CULL_NONE | POLY_ID(3));
			glSprite(200, 0,  GL_FLIP_NONE,           &Enemies[87 + PhoenixFrame]);
			glColor( RGB15(31,0,0) );
			glSprite(200, 30,  GL_FLIP_H,              &Enemies[87 + PhoenixFrame]);
			
			// Make the last two sprites translucent
			glPolyFmt(POLY_ALPHA(20) | POLY_CULL_NONE | POLY_ID(4));
			glColor( RGB15(0,31,20) );
			glSprite(200, 60,  GL_FLIP_V,              &Enemies[87 + PhoenixFrame]);
			glColor( RGB15(0,0,0) );
			glSprite(200, 90, GL_FLIP_V | GL_FLIP_H , &Enemies[87 + PhoenixFrame]);
			
			//Restore color and translucency to normal 
			glColor( RGB15(31,31,31) );
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(5));
			
			
			// "Clean Stretch" the sprite
			// Useful for lasers and some effects
			glSpriteStretchHorizontal( 0, 135, 64 + (abs(sinLerp(Frame*100) * 200) >> 12), Shuttle );
			
			
			// USING DIFFERENT PALETTES
			// Set the active texture to Zero
			// and use our special all white palette
			glSetActiveTexture( ZeroTextureID );
			glAssignColorTable( 0, PaletteID );
			
			// Zero Sprite is drawn all white
			glSprite( 0, 42*0, GL_FLIP_NONE, &Zero[ZeroFrame] );
			
			
			
			// Draw a horizontally  flipped "disco" zero
			// Disco fx is done with glColor
			int color = (Frame*4) & 31;
			glColor( RGB15(color, 31-color, 16+color*2) );
			glSprite( 0, 42*1, GL_FLIP_H, &Zero[ZeroFrame] );
			
			// Restore our palette
			glAssignColorTable( 0, OriginalPaletteID);
			
			// restore pal to enemies
			glColor( RGB15(31-color, 16 + color*2, color) );
			glSprite( 0, 42*2, GL_FLIP_V, &Zero[ZeroFrame] );
			
			
			// Normalize color
			glColor( RGB15(31, 31, 31) );
			glSprite( 0, 42*3, GL_FLIP_V |GL_FLIP_H, &Zero[ZeroFrame] );
			
			
			
		glEnd2D();

		
		
		glFlush( 0 );

		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
	
		
	}

	return 0;


}//end main 


// Draws the background
void DrawBG( glImage *images )
{

	int x, y, i;
	
	for( y = 0; y < 256 / 16; y++ )
	{
		for( x = 0; x < 256 / 16; x++ )
		{
			i = y * 16 + x;
			glSprite( x * 16, y * 16, GL_FLIP_NONE, &images[i & 255] );
		}
	}
	
	
}
