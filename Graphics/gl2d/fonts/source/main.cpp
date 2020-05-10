/******************************************************************************
*******************************************************************************
	Easy GL2D 
	Fonts example
	A simple font class for Easy GL2D DS
	
	Relminator (Richard Eric M. Lope BSN RN)
	Http://Rel.Phatcode.Net
	
	Thanks to:
	Adigun A. Polack for the fonts
	
*******************************************************************************
******************************************************************************/ 


#include <nds.h>
#include <stdio.h>
#include <gl2d.h>



// Our font class
// I decided to use c++ because of function overloading
class Cglfont
{
	public:
	
	Cglfont();
	int Load( glImage              *_font_sprite,
              const unsigned int   numframes, 
			  const unsigned int   *texcoords,
			  GL_TEXTURE_TYPE_ENUM type,
			  int 	               sizeX,
		      int 	               sizeY,
		      int 	               param,
		      int				   pallette_width,
			  const u16			   *palette,
			  const uint8          *texture	 
            );
	void Print( int x, int y, const char *text );
	void Print( int x, int y, int value );
	void PrintCentered( int x, int y, const char *text );
	void PrintCentered( int x, int y, int value );
	
	
	private:
	glImage     *font_sprite;
	char		str[256];
	char		str2[256];
};



Cglfont::Cglfont()
{

}

/******************************************************************************

	Loads our font

******************************************************************************/
int Cglfont::Load( glImage              *_font_sprite,
				   const unsigned int   numframes, 
				   const unsigned int   *texcoords,
				   GL_TEXTURE_TYPE_ENUM type,
				   int 	              	sizeX,
				   int 	              	sizeY,
				   int 	              	param,
				   int					pallette_width,
				   const u16			*palette,
				   const uint8          *texture	 
				 )

{
	font_sprite = _font_sprite;
	
	int textureID = 
		glLoadSpriteSet( font_sprite,
						 numframes, 
						 texcoords,
						 type,
						 sizeX,
						 sizeY,
						 param,
						 pallette_width,
					     palette,
						 texture	 
					   );
					   
	return textureID;

}

/******************************************************************************

	Prints a string

******************************************************************************/
void Cglfont::Print( int x, int y, const char *text )
{

	unsigned char font_char;
	
	while( *text )
	{
		font_char = ( *(unsigned char*)text++ ) - 32;
		glSprite( x, y, GL_FLIP_NONE, &font_sprite[font_char] );
		x += font_sprite[font_char].width; 
	}
	
}

/******************************************************************************

	Prints a number

******************************************************************************/
void Cglfont::Print( int x, int y, int value )
{

	
	sprintf( str,"%i",value );
	
	Print( x, y, str );
	
}

/******************************************************************************

	Center Prints a string

******************************************************************************/
void Cglfont::PrintCentered( int x, int y, const char *text )
{

	unsigned char font_char;
	int total_width = 0;
	char *o_text = (char*)text;
	
	while( *text )
	{
		font_char = ( *(unsigned char*)text++ ) - 32;
		total_width += font_sprite[font_char].width; 
	}
	
	x = (SCREEN_WIDTH - total_width) / 2; 
	
	text = o_text;
	while( *text )
	{
		font_char = (*(unsigned char*)text++) - 32;
		glSprite( x, y, GL_FLIP_NONE, &font_sprite[font_char] );
		x += font_sprite[font_char].width; 
	}
	
}

/******************************************************************************

	Center Prints a number

******************************************************************************/
void Cglfont::PrintCentered( int x, int y, int value )
{

	
	sprintf( str,"%i",value );
	
	PrintCentered( x, y, str );
	
}


/******************************************************************************

    MAIN 

******************************************************************************/



// GRIT auto-genrated arrays of images
#include "font_si.h"
#include "font_16x16.h"

// Texture Packer auto-generated UV coords
#include "uvcoord_font_si.h"
#include "uvcoord_font_16x16.h"

// This imageset would use our texture packer generated coords so it's kinda
// safe and easy to use 
// FONT_SI_NUM_IMAGES is a value #defined from "uvcoord_font_si.h"
glImage  FontImages[FONT_SI_NUM_IMAGES];
glImage  FontBigImages[FONT_16X16_NUM_IMAGES];


// Our fonts
Cglfont Font;
Cglfont FontBig;
	


int main( int argc, char *argv[] )
{

	
	
	// Set it to my favorite mode
	videoSetMode(MODE_5_3D);
	
	
	consoleDemoInit();
	
	
	// Initialize GL in 3d mode
	glScreen2D();
	
	
	// Set  Bank A to texture (128 kb)
	vramSetBankA( VRAM_A_TEXTURE );
	

	vramSetBankE(VRAM_E_TEX_PALETTE);  // Allocate VRAM bank for all the palettes
	
	// Load our font texture
	// We used glLoadSpriteSet since the texture was made
	// with my texture packer.
	// no need to save the return value since
	// we don't need  it at all
	Font.Load( FontImages,				// pointer to glImage array
			   FONT_SI_NUM_IMAGES, 		// Texture packer auto-generated #define
			   font_si_texcoords,		// Texture packer auto-generated array
			   GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
			   TEXTURE_SIZE_64,			// sizeX for glTexImage2D() in videoGL.h
			   TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
			   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
			   256,						// Length of the palette (256 colors)
			   (u16*)font_siPal,		// Palette Data
			   (u8*)font_siBitmap		// image data generated by GRIT 
			 );

	// Do the same with our bigger texture
	FontBig.Load( FontBigImages,
			   FONT_16X16_NUM_IMAGES, 
			   font_16x16_texcoords,
			   GL_RGB256,
			   TEXTURE_SIZE_64,
			   TEXTURE_SIZE_512,
			   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
			   256,
			   (u16*)font_siPal,
			   (u8*)font_16x16Bitmap	 
			 );


	
	
	iprintf("\x1b[1;1HEasy GL2D Font Example");
	iprintf("\x1b[3;1HFonts by Adigun A. Polack");
	
	iprintf("\x1b[6;1HRelminator");
	iprintf("\x1b[7;1HHttp://Rel.Phatcode.Net");
	
	// calculate the amount of 
	// memory uploaded to VRAM in KB
	int TextureSize = font_siBitmapLen +
					  font_16x16BitmapLen;
					  
					  
	iprintf("\x1b[10;1HTotal Texture size= %i kb", TextureSize / 1024);
	
	// our ever present frame counter	
	int frame = 0;
	
	
	while( 1 )
	{
		// increment frame counter and rotation offsets
	
		frame++;
	
		// set up GL2D for 2d mode
		glBegin2D();
			
			// fill the whole screen with a gradient box
			glBoxFilledGradient( 0, 0, 255, 191,
								 RGB15( 31,  0,  0 ),
								 RGB15(  0, 31,  0 ),
								 RGB15( 31,  0, 31 ),
								 RGB15(  0, 31, 31 )
                               );
			
			// Center print the title
			glColor( RGB15(0,0,0) );
			FontBig.PrintCentered( 0, 0, "EASY GL2D" );
			glColor( RGB15((frame*6)&31,(-frame*4)&31, (frame*2)&31) );
			FontBig.PrintCentered( 0, 20,  "FONT EXAMPLE" );

			// Fixed-point sinusoidal movement
			int x = ( sinLerp( frame * 400 ) * 30 ) >> 12;
	   
			// Make the fonts sway left and right
			// Also change coloring of fonts
			glColor( RGB15(31,0,0) );
			FontBig.Print( 25 + x, 50, "hfDEVKITPROfh" );
			glColor( RGB15(31,0,31) );
			glColor( RGB15(x, 31 - x, x * 2) );
			FontBig.Print( 50 - x, 70, "dcLIBNDScd" );
			
			
			
			// change fontsets and  print some spam
			glColor( RGB15(0,31,31) );
			Font.PrintCentered( 0, 100, "FONTS BY ADIGUN A. POLACK" );
			Font.PrintCentered( 0, 120, "CODE BY RELMINATOR" );
			
			
			// Restore normal coloring
			glColor( RGB15(31,31,31) );
			
			// Change opacity relative to frame
			int opacity = abs( sinLerp( frame * 245 ) * 30 ) >> 12;
			
			// translucent mode 
			// Add 1 to opacity since at 0 we will get into wireframe mode
			glPolyFmt(POLY_ALPHA(1 + opacity) | POLY_CULL_NONE | POLY_ID(1));
			FontBig.Print( 35 + x, 140, "ANYA THERESE" );
			
			
			
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(2));
			
			// Print the number of frames
			Font.Print( 10, 170, "FRAMES = " );
			Font.Print( 10 + 72, 170, frame );
			
			
		glEnd2D();
		
		glFlush(0);

		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
		
	
	}

	return 0;
	
}


