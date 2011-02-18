#include <nds.h>
#include <stdlib.h>
#include <stdio.h>
#include <nds/arm9/console.h> //basic print funcionality


//texture_bin our original texture
#include "texture_bin.h"

// most of the following textures were generated from online samples available at 
// http://www.marlinstudios.com/samples/sampvtf.htm and http://www.3dtotal.com/textures_v15/

#include "texture1_RGB16_pal_bin.h"
#include "texture1_RGB16_tex_bin.h"
#include "texture2_RGB16_pal_bin.h"
#include "texture2_RGB16_tex_bin.h"
#include "texture3_RGB16_pal_bin.h"
#include "texture3_RGB16_tex_bin.h"
#include "texture4_RGB16_pal_bin.h"
#include "texture4_RGB16_tex_bin.h"
#include "texture5_RGB16_pal_bin.h"
#include "texture5_RGB16_tex_bin.h"
#include "texture6_RGB4_pal_bin.h"
#include "texture6_RGB4_tex_bin.h"
#include "texture7_RGB4_pal_bin.h"
#include "texture7_RGB4_tex_bin.h"
#include "texture8_RGB32_A3_pal_bin.h"
#include "texture8_RGB32_A3_tex_bin.h"
#include "texture9_RGB32_A3_pal_bin.h"
#include "texture9_RGB32_A3_tex_bin.h"

// I wish I could remember where I got this compressed texture from, but I'm sure that someone
// who sees it might know where it comes from.

#include "texture10_COMP_tex_bin.h"
#include "texture10_COMP_texExt_bin.h"
#include "texture10_COMP_pal_bin.h"



//verticies for the cube
v16 CubeVectors[] = {
	floattov16(-0.5), floattov16(-0.5), floattov16(0.5), 
	floattov16(0.5),  floattov16(-0.5), floattov16(0.5),
	floattov16(0.5),  floattov16(-0.5), floattov16(-0.5),
	floattov16(-0.5), floattov16(-0.5), floattov16(-0.5),
	floattov16(-0.5), floattov16(0.5),  floattov16(0.5), 
	floattov16(0.5),  floattov16(0.5),	floattov16(0.5),
	floattov16(0.5),  floattov16(0.5),  floattov16(-0.5),
	floattov16(-0.5), floattov16(0.5),  floattov16(-0.5)
};

//polys
u8 CubeFaces[] = {
	3,2,1,0,
	0,1,5,4,
	1,2,6,5,
	2,3,7,6,
	3,0,4,7,
	5,6,7,4
};

//texture coordinates
u32 uv[] =
{

	TEXTURE_PACK(inttot16(128), 0),
	TEXTURE_PACK(inttot16(128),inttot16(128)),
	TEXTURE_PACK(0, inttot16(128)),
	TEXTURE_PACK(0,0)
};

u32 normals[] =
{
	NORMAL_PACK(0,floattov10(-.97),0),
	NORMAL_PACK(0,0,floattov10(.97)),
	NORMAL_PACK(floattov10(.97),0,0),
	NORMAL_PACK(0,0,floattov10(-.97)),
	NORMAL_PACK(floattov10(-.97),0,0),
	NORMAL_PACK(0,floattov10(.97),0)

};

//draw a cube face at the specified color
void drawQuad(int poly)
{	

	u32 f1 = CubeFaces[poly * 4] ;
	u32 f2 = CubeFaces[poly * 4 + 1] ;
	u32 f3 = CubeFaces[poly * 4 + 2] ;
	u32 f4 = CubeFaces[poly * 4 + 3] ;


	glNormal(normals[poly]);

	GFX_TEX_COORD = (uv[0]);
	glVertex3v16(CubeVectors[f1*3], CubeVectors[f1*3 + 1], CubeVectors[f1*3 +  2] );

	GFX_TEX_COORD = (uv[1]);
	glVertex3v16(CubeVectors[f2*3], CubeVectors[f2*3 + 1], CubeVectors[f2*3 + 2] );

	GFX_TEX_COORD = (uv[2]);
	glVertex3v16(CubeVectors[f3*3], CubeVectors[f3*3 + 1], CubeVectors[f3*3 + 2] );

	GFX_TEX_COORD = (uv[3]);
	glVertex3v16(CubeVectors[f4*3], CubeVectors[f4*3 + 1], CubeVectors[f4*3 + 2] );
}


int main()
{	
	int textureIDS[11];
	int paletteIDS[2];
	int i;
	float rotateX = 0.0;
	float rotateY = 0.0;

	lcdMainOnTop();

	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);

	//Because of letting the user manipulate which video banks the program will use,
	// I chose to manually set the console data into Bank I, as the demo default uses Bank C.
	videoSetModeSub( MODE_0_2D  );
	vramSetBankI( VRAM_I_SUB_BG_0x06208000 );
	consoleInit( NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 23, 2, false, true );


	// initialize gl
	glInit();

	//enable textures
	glEnable(GL_TEXTURE_2D);

	// enable antialiasing
	glEnable(GL_ANTIALIAS);

	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call
	glViewport(0,0,255,191);

	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();

	// setup other material properties
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(20,20,20));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(5,5,5));

	// setup the lighting
	glLight(0, RGB15(31,31,31) , 0, floattov10(-.5), floattov10(-.85));


	//You may comment/uncomment what you like, as the integration of nglVideo into libnds works
	// by examining the state of the banks, and deciding where to put textures/texpalettes based on that.
	//There are some exceptions to get certain stuff working though...
	// At least one main bank (A-D) must be allocated to textures to load/use them obviously, as well as 
	//  sub banks (E-G) for texture palettes
	// Compressed textures require bank B allocated, as well as bank A or C (or both) to be loadable/usable
	// 4 color palettes (not 4-bit) require either bank E, or bank F/G as slot0/1


	//vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankC(VRAM_C_TEXTURE);
	//vramSetBankD(VRAM_D_TEXTURE);
	//vramSetBankE(VRAM_E_TEX_PALETTE);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);
	vramSetBankG(VRAM_G_TEX_PALETTE_SLOT5);



	glGenTextures(11, textureIDS);

	// inital full 16 bit colour texture
	glBindTexture(0, textureIDS[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture_bin);


	// Load a 16 colour texture
	glBindTexture(0, textureIDS[1]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture1_RGB16_tex_bin);

	//create two alternate textures for that texture.
	//this illustrates our recommended approach for palette-swap management, which is to consider them as separate resources
	//that just so happen to be allocated with glGenTextures()
	u16 tempPalette[16];
	glGenTextures(2, &paletteIDS[0]);
	//make a green-tinted one
	memcpy(tempPalette,texture1_RGB16_pal_bin,32);
	for(int i=0;i<16;i++) tempPalette[i] |= RGB15(0,31,0);
	glBindTexture(0, paletteIDS[0]); 
	glColorTableEXT(0,0,16,0,0,tempPalette);
	//make a blue-tinted one
	memcpy(tempPalette,texture1_RGB16_pal_bin,32);
	for(int i=0;i<16;i++) tempPalette[i] |= RGB15(0,0,31);
	glBindTexture(0, paletteIDS[1]); 
	glColorTableEXT(0,0,16,0,0,tempPalette);


	// Just to show that this works, let's go and delete that very first texture that was loaded
	glDeleteTextures( 1, &textureIDS[ 0 ] );	


	// Load some more 16 color textures
	glBindTexture(0, textureIDS[2]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture2_RGB16_tex_bin);
	glColorTableEXT( 0, 0, 16, 0, 0, (u16*)texture2_RGB16_pal_bin );

	glBindTexture(0, textureIDS[3]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture3_RGB16_tex_bin);
	glColorTableEXT( 0, 0, 16, 0, 0, (u16*)texture3_RGB16_pal_bin );


	// Now, re-generate the first texture, who's VRAM position won't be the same as before in the end
	glGenTextures( 1, &textureIDS[ 0 ] );


	// Load some more 16 color textures
	glBindTexture(0, textureIDS[4]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture4_RGB16_tex_bin);
	glColorTableEXT( 0, 0, 16, 0, 0, (u16*)texture4_RGB16_pal_bin );

	glBindTexture(0, textureIDS[5]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture5_RGB16_tex_bin);
	glColorTableEXT( 0, 0, 16, 0, 0, (u16*)texture5_RGB16_pal_bin );


	// Load some 4 colour textures
	glBindTexture(0, textureIDS[6]);
	glTexImage2D(0, 0, GL_RGB4, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture6_RGB4_tex_bin);
	glColorTableEXT( 0, 0, 4, 0, 0, (u16*)texture6_RGB4_pal_bin );

	glBindTexture(0, textureIDS[7]);
	glTexImage2D(0, 0, GL_RGB4, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture7_RGB4_tex_bin);
	glColorTableEXT( 0, 0, 4, 0, 0, (u16*)texture7_RGB4_pal_bin );


	// Load some 32 colour textures, 8 levels of alpha
	glBindTexture(0, textureIDS[8]);
	glTexImage2D(0, 0, GL_RGB32_A3, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture8_RGB32_A3_tex_bin);
	glColorTableEXT( 0, 0, 32, 0, 0, (u16*)texture8_RGB32_A3_pal_bin );

	glBindTexture(0, textureIDS[9]);
	glTexImage2D(0, 0, GL_RGB32_A3, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture9_RGB32_A3_tex_bin);
	glColorTableEXT( 0, 0, 32, 0, 0, (u16*)texture9_RGB32_A3_pal_bin );


	// Load a 4x4 texel compressed texture
	// The tiles and header need to be combined together in that order
	// If this data is already pre-combined together, then you can just send it into the nglTexImage2D function
	u8 *compTexture = (u8*)malloc( texture10_COMP_tex_bin_size + texture10_COMP_texExt_bin_size );
	swiCopy( texture10_COMP_tex_bin, compTexture, texture10_COMP_tex_bin_size >> 2 | COPY_MODE_WORD );
	swiCopy( texture10_COMP_texExt_bin, compTexture + texture10_COMP_tex_bin_size , texture10_COMP_texExt_bin_size >> 2 | COPY_MODE_WORD );

	glBindTexture(0, textureIDS[10]);
	glTexImage2D(0, 0, GL_COMPRESSED, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)compTexture);
	glColorTableEXT( 0, 0, texture10_COMP_pal_bin_size >> 1, 0, 0, (u16*)texture10_COMP_pal_bin );


	// Now, let's reload the full 16 bit color texture was had it's name deleted and regenerated
	glBindTexture(0, textureIDS[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture_bin);



	iprintf("\x1b[4;8HPaletted Cube");
	iprintf("\x1b[6;2HRight/Left shoulder to switch");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);

	gluLookAt(	0.0, 0.0, 2.0,		//camera possition 
		0.0, 0.0, 0.0,		//look at
		0.0, 1.0, 0.0);		//up

	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0 | POLY_ID(1) ) ;

	glColor3f(1,1,1);

	glMatrixMode(GL_MODELVIEW);

	int nTexture = 0;
	while(1)		
	{
		glPushMatrix();

		glRotateX(rotateX);
		glRotateY(rotateY);

		scanKeys();
		u16 keys = keysHeld();
		if((keys & KEY_UP)) rotateX += 3;
		if((keys & KEY_DOWN)) rotateX -= 3;
		if((keys & KEY_LEFT)) rotateY += 3;
		if((keys & KEY_RIGHT)) rotateY -= 3;

		u16 keysPressed = keysDown();
		if(keysPressed & KEY_R)
		{
			if( ++nTexture == 11 )	
				nTexture=0;
		}
		if(keysPressed & KEY_L) 
		{
			if( --nTexture == -1 )	
				nTexture=10;
		}

		glBindTexture(nTexture, textureIDS[nTexture]);

		//draw the obj
		glBegin(GL_QUAD);
		for(i = 0; i < 6; i++)
		{
			if(nTexture==1)
			{
				//assign alternating palettes for texture 1, which is where we have setup the palette swapping demo
				glAssignColorTable(0,paletteIDS[i&1]);
			}
			drawQuad(i);
		}
		glEnd();

		glPopMatrix(1);

		glFlush(0);
		swiWaitForVBlank();
	}

	return 0;
}//end main 
