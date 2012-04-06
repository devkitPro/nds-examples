#include <nds.h>
#include <stdlib.h>
#include <stdio.h>

#include "i2.h" //deleted
//tinted
#include "i4.h"
#include "i8.h"
#include "16bpp.h"
#include "a3i5.h"
#include "a5i3.h"
//compressed

static const char* test_names[] = {
	"i2 (GL_RGB4)",
	"i2 (GL_RGB4) palette-swapped",
	"i4 (GL_RGB16)",
	"i8 (GL_RGB256)",
	"direct color (GL_RGB)",
	"a3i5 (GL_RGB32_A3)",
	"a5i3 (GL_RGB8_A5)",
	"4x4 compressed (GL_COMPRESSED)"
};

// I wish I could remember where I got this compressed texture from, but I'm sure that someone
// who sees it might know where it comes from.
#include "texture10_COMP_tex_bin.h"
#include "texture10_COMP_texExt_bin.h"
#include "texture10_COMP_pal_bin.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

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

	//not using lighting; not using normals
	//glNormal(normals[poly]);

	GFX_TEX_COORD = (uv[0]);
	glVertex3v16(CubeVectors[f1*3], CubeVectors[f1*3 + 1], CubeVectors[f1*3 +  2] );

	GFX_TEX_COORD = (uv[1]);
	glVertex3v16(CubeVectors[f2*3], CubeVectors[f2*3 + 1], CubeVectors[f2*3 + 2] );

	GFX_TEX_COORD = (uv[2]);
	glVertex3v16(CubeVectors[f3*3], CubeVectors[f3*3 + 1], CubeVectors[f3*3 + 2] );

	GFX_TEX_COORD = (uv[3]);
	glVertex3v16(CubeVectors[f4*3], CubeVectors[f4*3 + 1], CubeVectors[f4*3 + 2] );
}

void TintPalette(u16* dst, u16* src, int count, int r, int g, int b)
{
	memcpy(dst,src,count*2);
	for(int i=0;i<count;i++) dst[i] |= RGB15(r,g,b);
}

int main()
{	
	consoleDebugInit(DebugDevice_NOCASH);

	int textureIDS[8];
	int paletteIDS[6];
	float rotateX = 0.0;
	float rotateY = 0.0;

	lcdMainOnTop();

	videoSetMode(MODE_0_3D);
	consoleInit( NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 23, 2, false, true );
	consoleDemoInit();

	// initialize gl
	glInit();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ANTIALIAS);
	glEnable(GL_BLEND);

	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call
	glViewport(0,0,255,191);

	//You may comment/uncomment what you like, as the integration of nglVideo into libnds works
	// by examining the state of the banks, and deciding where to put textures/texpalettes based on that.
	//There are some exceptions to get certain stuff working though...
	// At least one main bank (A-D) must be allocated to textures to load/use them obviously, as well as 
	//  sub banks (E-G) for texture palettes
	// Compressed textures require bank B allocated, as well as bank A or C (or both) to be loadable/usable
	// 4 color palettes (not 4-bit) require either bank E, or bank F/G as slot0/1


	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	//vramSetBankC(VRAM_C_TEXTURE);
	//vramSetBankD(VRAM_D_TEXTURE);
	//vramSetBankE(VRAM_E_TEX_PALETTE);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);
	vramSetBankG(VRAM_G_TEX_PALETTE_SLOT5);


	glGenTextures(ARRAY_SIZE(textureIDS), textureIDS);

	//I2
	glBindTexture(0, textureIDS[0]);
	glTexImage2D(0, 0, GL_RGB4, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)i2Bitmap);

	//create two alternate textures for that texture.
	//this illustrates our recommended approach for palette-swap management, which is to consider them as separate resources
	//that just so happen to be allocated with glGenTextures()
	glBindTexture(0, textureIDS[1]);
	glTexImage2D(0, 0, GL_RGB4, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)i2Bitmap);
	u16 tempPalette[4];
	glGenTextures(6, &paletteIDS[0]);
	
	static const int tintColors[][3] = {
		{0,31,0},
		{0,0,31},
		{31,0,0},
		{31,0,31},
		{31,31,0},
		{0,31,31}
	};

	for(int i=0;i<6;i++)
	{
		TintPalette(tempPalette,(u16*)i2Pal,4, tintColors[i][0], tintColors[i][1], tintColors[i][2]);
		glBindTexture(0, paletteIDS[i]); 
		glColorTableEXT(0,0,4,0,0,tempPalette);
	}

	// Just to show that this works, let's go and delete that very first texture that was loaded and load it again
	glDeleteTextures( 1, &textureIDS[ 0 ] );	
	
	//I2 (tinted)
	glBindTexture(0, textureIDS[1]);
	glTexImage2D(0, 0, GL_RGB4, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)i2Bitmap);
	glColorTableEXT( 0, 0, 4, 0, 0, (u16*)i2Pal );

	//I4
	glBindTexture(0, textureIDS[2]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)i4Bitmap);
	glColorTableEXT( 0, 0, 16, 0, 0, (u16*)i4Pal );

	//I8
	glBindTexture(0, textureIDS[3]);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)i8Bitmap);
	glColorTableEXT( 0, 0, 256, 0, 0, (u16*)i8Pal );

	// Now, re-generate the first texture, who's VRAM position won't be the same as before in the end
	glGenTextures( 1, &textureIDS[ 0 ] );

	//16bpp
	glBindTexture(0, textureIDS[4]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)_6bppBitmap);

	//A3I5
	glBindTexture(0, textureIDS[5]);
	glTexImage2D(0, 0, GL_RGB32_A3, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)a3i5Bitmap);
	glColorTableEXT( 0, 0, 32, 0, 0, (u16*)a3i5Pal );

	//A5I3
	glBindTexture(0, textureIDS[6]);
	glTexImage2D(0, 0, GL_RGB8_A5, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)a5i3Bitmap);
	glColorTableEXT( 0, 0, 8, 0, 0, (u16*)a5i3Pal );



	// Load a 4x4 texel compressed texture
	// The tiles and header need to be combined together in that order
	// If this data is already pre-combined together, then you can just send it into the nglTexImage2D function
	u8 *compTexture = (u8*)malloc( texture10_COMP_tex_bin_size + texture10_COMP_texExt_bin_size );
	swiCopy( texture10_COMP_tex_bin, compTexture, texture10_COMP_tex_bin_size >> 2 | COPY_MODE_WORD );
	swiCopy( texture10_COMP_texExt_bin, compTexture + texture10_COMP_tex_bin_size , texture10_COMP_texExt_bin_size >> 2 | COPY_MODE_WORD );

	glBindTexture(0, textureIDS[7]);
	glTexImage2D(0, 0, GL_COMPRESSED, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)compTexture);
	glColorTableEXT( 0, 0, texture10_COMP_pal_bin_size >> 1, 0, 0, (u16*)texture10_COMP_pal_bin );


	//I2 (recreated after getting deleted)
	glBindTexture(0, textureIDS[0]);
	glTexImage2D(0, 0, GL_RGB4, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)i2Bitmap);
	glColorTableEXT( 0, 0, 4, 0, 0, (u16*)i2Pal );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);

	float fCamera = 1.25;
	int nTexture = 0;

	for(;;)
	{

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		gluLookAt(	0.0, 0.0, fCamera,		//camera possition 
			0.0, 0.0, 0.0,		//look at
			0.0, 1.0, 0.0);		//up



		glRotateX(rotateX);
		glRotateY(rotateY);

		scanKeys();
		u16 keys = keysHeld();
		if((keys & KEY_UP)) rotateX += 3;
		if((keys & KEY_DOWN)) rotateX -= 3;
		if((keys & KEY_LEFT)) rotateY += 3;
		if((keys & KEY_RIGHT)) rotateY -= 3;
		if((keys & KEY_A)) fCamera -= 0.05f;
		if((keys & KEY_B)) fCamera += 0.05f;

		if(fCamera <= 0.58f) fCamera = 0.58f;

		u16 keysPressed = keysDown();
		if(keysPressed & KEY_R)
		{
			if( ++nTexture == ARRAY_SIZE(textureIDS) )	
				nTexture=0;
		}
		if(keysPressed & KEY_L) 
		{
			if( --nTexture == -1 )	
				nTexture=ARRAY_SIZE(textureIDS)-1;
		}

		glBindTexture(nTexture, textureIDS[nTexture]);

		//draw the obj
		glColor3b(255,255,255);
		glScalef(0.4f,0.4f,0.4f);
		int polyid = 1;
		for(int j = 0; j < 2; j++)
		{
			for(int i = 0; i < 6; i++)
			{
				if(nTexture==1)
				{
					//assign various palettes for texture 1, which is where we have setup the palette swapping demo
					glAssignColorTable(0,paletteIDS[i]);
				}
				if(nTexture == 3)
				{
					//glTexParameter(0, 0); //disable GL_TEXTURE_COLOR0_TRANSPARENT
				}

				glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_MODULATION | POLY_ID(polyid) ) ;
				glBegin(GL_QUAD);
				drawQuad(i);
				glEnd();				
				polyid++;
			}

			glScalef(1.0f/0.4f,1.0f/0.4f,1.0f/0.4f);
		}

		glPopMatrix(1);

		glFlush(GL_TRANS_MANUALSORT);

		consoleClear();
		iprintf("test %d:\n %s\n",nTexture, test_names[nTexture]);
		printf("\nrot: %f, %f\n",rotateX,rotateY);
		printf("cam: %f\n",fCamera);
		iprintf("\nuse d-pad to rotate\n");
		iprintf("use L/R to change test\n");
		iprintf("use A/B to zoom\n");

		swiWaitForVBlank();
	}

	return 0;
}//end main 
