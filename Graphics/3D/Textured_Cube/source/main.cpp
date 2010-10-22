#include <nds.h>
#include <stdlib.h>

//texture_bin.h is created automagicaly from the texture.bin placed in arm9/resources
//texture.bin is a raw 128x128 16 bit image.  I will release a tool for texture conversion 
//later
#include "texture_bin.h"


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

void DisplayEnableMotionBlur()
{
	u32 dispcnt = REG_DISPCNT;
	//set main display to display from VRAM
	dispcnt &= ~(0x00030000); dispcnt |= 2<<16; //choose to display screen from VRAM
	dispcnt &= ~(0x000C0000); dispcnt |= 1<<18; //choose to display screen from VRAM_B
	REG_DISPCNT = dispcnt;
}

void DisplayEnableNormal()
{
	u32 dispcnt = REG_DISPCNT;
	dispcnt &= ~(0x00030000); dispcnt |= 1<<16; //choose to display screen from normal layer composition
	REG_DISPCNT = dispcnt;
}

int main()
{	
	
	int textureID;
	int i;
	float rotateX = 0.0;
	float rotateY = 0.0;

	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);

	// initialize gl
	glInit();
	
	//enable textures
	glEnable(GL_TEXTURE_2D);
	
	//this should work the same as the normal gl call
	glViewport(0,0,255,191);
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	vramSetBankB(VRAM_B_LCD);
	REG_DISPCAPCNT = 
			DCAP_MODE(DCAP_MODE_BLEND) //blend source A and source B
		//|	DCAP_SRC_ADDR //this is not used since we are setting the display to render from VRAM
		|	DCAP_SRC_B(DCAP_SRC_B_VRAM)
		|	DCAP_SRC_A(DCAP_SRC_A_3DONLY)
		|	DCAP_SIZE(DCAP_SIZE_256x192)
		|	DCAP_OFFSET(0) //where to write the captured data within our chosen VRAM bank
		|	DCAP_BANK(DCAP_BANK_VRAM_B)
		|	DCAP_B(12) //blend mostly from B to make a very dramatic effect
		|	DCAP_A(4) //and blend only a little bit from the new scene
		;
	//but, dramatic effects tend to leave some garbage on the screen since the precision of the math is low,
	//and we're not putting a lot of dampening on the effect.
	//a more realistic value might be 8 and 8, but perhaps in a more complex 3d scene the garbage isn't such a bad thing
	//since the scene is changing constantly

	DisplayEnableMotionBlur();
	bool displayBlurred = true;


	vramSetBankA(VRAM_A_TEXTURE);

	glGenTextures(1, &textureID);
	glBindTexture(0, textureID);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture_bin);
	
	
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	gluLookAt(	0.0, 0.0, 1.0,		//camera possition 
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
	
	while(1) {
		
		glLight(0, RGB15(31,31,31) , 0,				  floattov10(-1.0),		 0);
		glLight(1, RGB15(31,0,31),   0,				  floattov10(1) - 1,			 0);
		glLight(2, RGB15(0,31,0) ,   floattov10(-1.0), 0,					 0);
		glLight(3, RGB15(0,0,31) ,   floattov10(1.0) - 1,  0,					 0);

		glPushMatrix();

		//move it away from the camera
		glTranslatef32(0, 0, floattof32(-1));
				
		glRotateX(rotateX);
		glRotateY(rotateY);
		
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		
		glMatrixMode(GL_MODELVIEW);

		glMaterialf(GL_AMBIENT, RGB15(8,8,8));
		glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
		glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
		glMaterialf(GL_EMISSION, RGB15(5,5,5));

		//ds uses a table for shinyness..this generates a half-ass one
		glMaterialShinyness();

		//not a real gl function and will likely change
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | 
													POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3 ) ;
		
		scanKeys();
		
		u16 keys = keysHeld();
		
		if((keys & KEY_UP)) rotateX += 3;
		if((keys & KEY_DOWN)) rotateX -= 3;
		if((keys & KEY_LEFT)) rotateY += 3;
		if((keys & KEY_RIGHT)) rotateY -= 3;

		if(keysDown() & KEY_A)
		{
			displayBlurred = !displayBlurred;
			if(displayBlurred)
				DisplayEnableMotionBlur();
			else 
				DisplayEnableNormal();
		}
		
		glBindTexture(0, textureID);

		//draw the obj
		glBegin(GL_QUAD);
			for(i = 0; i < 6; i++)
				drawQuad(i);
		
		glEnd();
		
		glPopMatrix(1);
			
		glFlush(0);

		swiWaitForVBlank();

		//the display capture enable bit must be set again each frame if you want to continue capturing.
		REG_DISPCAPCNT |= DCAP_ENABLE;
	}

	return 0;
}//end main 
