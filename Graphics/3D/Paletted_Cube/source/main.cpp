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
	
	int textureIDS[10];
	struct { int format, pal_addr; u32 size; } textures[10];
	int i;
	float rotateX = 0.0;
	float rotateY = 0.0;

	lcdMainOnTop();
	
	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);
	consoleDemoInit();

	
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
	
	vramSetBankA(VRAM_A_TEXTURE);

	glGenTextures(10, textureIDS);
	
	// inital full 16 bit colour texture
	glBindTexture(0, textureIDS[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture_bin);
	textures[0].format = GL_RGB;
	textures[0].pal_addr = 0;	
	textures[0].size = texture_bin_size; 	// size field just recorded for on-screen info
	

	// Load some 16 colour textures
	glBindTexture(0, textureIDS[1]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture1_RGB16_tex_bin);
	textures[1].format = GL_RGB16;
	textures[1].pal_addr = gluTexLoadPal( (u16*)texture1_RGB16_pal_bin, 16, GL_RGB16 );
	textures[1].size = texture1_RGB16_tex_bin_size+texture1_RGB16_pal_bin_size;

	glBindTexture(0, textureIDS[2]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture2_RGB16_tex_bin);
	textures[2].format = GL_RGB16;
	textures[2].pal_addr = gluTexLoadPal( (u16*)texture2_RGB16_pal_bin, 16, GL_RGB16 );
	textures[2].size = texture2_RGB16_tex_bin_size+texture2_RGB16_pal_bin_size;
	
	glBindTexture(0, textureIDS[3]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture3_RGB16_tex_bin);
	textures[3].format = GL_RGB16;
	textures[3].pal_addr = gluTexLoadPal( (u16*)texture3_RGB16_pal_bin, 16, GL_RGB16 );
	textures[3].size = texture3_RGB16_tex_bin_size+texture3_RGB16_pal_bin_size;
	
	glBindTexture(0, textureIDS[4]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture4_RGB16_tex_bin);
	textures[4].format = GL_RGB16;
	textures[4].pal_addr = gluTexLoadPal( (u16*)texture4_RGB16_pal_bin, 16, GL_RGB16 );
	textures[4].size = texture4_RGB16_tex_bin_size+texture4_RGB16_pal_bin_size;
	
	glBindTexture(0, textureIDS[5]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture5_RGB16_tex_bin);
	textures[5].format = GL_RGB16;
	textures[5].pal_addr = gluTexLoadPal( (u16*)texture5_RGB16_pal_bin, 16, GL_RGB16 );
	textures[5].size = texture5_RGB16_tex_bin_size+texture5_RGB16_pal_bin_size;


	// Load some 4 colour textures
	glBindTexture(0, textureIDS[6]);
	glTexImage2D(0, 0, GL_RGB4, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture6_RGB4_tex_bin);
	textures[6].format = GL_RGB4;
	textures[6].pal_addr = gluTexLoadPal( (u16*)texture6_RGB4_pal_bin, 4, GL_RGB4 );
	textures[6].size = texture6_RGB4_tex_bin_size+texture6_RGB4_pal_bin_size;

	glBindTexture(0, textureIDS[7]);
	glTexImage2D(0, 0, GL_RGB4, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture7_RGB4_tex_bin);
	textures[7].format = GL_RGB4;
	textures[7].pal_addr = gluTexLoadPal( (u16*)texture7_RGB4_pal_bin, 4, GL_RGB4 );
	textures[7].size = texture7_RGB4_tex_bin_size+texture7_RGB4_pal_bin_size;


	// Load some 32 colour textures, 8 levels of alpha
	glBindTexture(0, textureIDS[8]);
	glTexImage2D(0, 0, GL_RGB32_A3, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture8_RGB32_A3_tex_bin);
	textures[8].format = GL_RGB32_A3;
	textures[8].pal_addr = gluTexLoadPal( (u16*)texture8_RGB32_A3_pal_bin, 32, GL_RGB32_A3 );
	textures[8].size = texture8_RGB32_A3_tex_bin_size+texture8_RGB32_A3_pal_bin_size;

	glBindTexture(0, textureIDS[9]);
	glTexImage2D(0, 0, GL_RGB32_A3, TEXTURE_SIZE_128, TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture9_RGB32_A3_tex_bin);
	textures[9].format = GL_RGB32_A3;
	textures[9].pal_addr = gluTexLoadPal( (u16*)texture9_RGB32_A3_pal_bin, 32, GL_RGB32_A3 );
	textures[9].size = texture9_RGB32_A3_tex_bin_size+texture9_RGB32_A3_pal_bin_size;
	
	

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
		if(!(keysPressed & KEY_R)) 
		{
			if( ++nTexture == 10 )	
				nTexture=0;
		}
		if(!(keysPressed & KEY_L)) 
		{
			if( --nTexture == -1 )	
				nTexture=9;
		}
		
		glBindTexture(nTexture, textureIDS[nTexture]);
		if( textures[nTexture].format != GL_RGB )
		{
			glColorTable(textures[nTexture].format, textures[nTexture].pal_addr);
		}

		//draw the obj
		glBegin(GL_QUAD);
			for(i = 0; i < 6; i++)
				drawQuad(i);
		glEnd();
		
		glPopMatrix(1);
			
		glFlush(0);
	}

	return 0;
}//end main 
