/****************************************
 * 		NDS NeHe Lesson 11    			*
 * 		Author: Dovoto					*
 ****************************************/

// include your ndslib
#include <nds.h>
#include <malloc.h>
#include <stdio.h>

//needed to load pcx files
#include <nds/arm9/image.h>
#include <nds/arm9/trig_lut.h>


#include "drunkenlogo_pcx.h"



int DrawGLScene();

v16 points[64][32][3];    // The Array For The Points On The Grid Of Our "Wave"
int wiggle_count = 0;		// Counter Used To Control How Fast Flag Waves

float	xrot;				// X Rotation ( NEW )
float	yrot;				// Y Rotation ( NEW )
float	zrot;				// Z Rotation ( NEW )
v16 hold;				// Temporarily Holds A Floating Point Value

int	texture[1];			// Storage For 3 Textures (only going to use 1 on the DS for this demo)



float sin(float angle)
{
	int32 s = sinLerp((int)((angle * DEGREES_IN_CIRCLE) / 360.0));

	return f32tofloat(s);
}

float cos(float angle)
{
	int32 c = cosLerp((int)((angle * DEGREES_IN_CIRCLE) / 360.0));

	return f32tofloat(c);
}
 
 
int LoadGLTextures()									// Load PCX files And Convert To Textures
{
	sImage pcx;                //////////////(NEW) and different from nehe.

	//load our texture
	loadPCX((u8*)drunkenlogo_pcx, &pcx);
	
	image8to16(&pcx);

	glGenTextures(1, &texture[0]);
	glBindTexture(0, texture[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, pcx.image.data8);

	return TRUE;
}
void InitGL(void) {
	
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE);                        //NEW  must set up some memory for textures
	
	// initialize the geometry engine
	glInit();
	
	// enable textures
	glEnable(GL_TEXTURE_2D);
	
	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	LoadGLTextures();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
	
	//need to set up some material properties since DS does not have them set by default
	glMaterialf(GL_AMBIENT, RGB15(31,31,31));
	glMaterialf(GL_DIFFUSE, RGB15(31,31,31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(16,16,16));
	glMaterialf(GL_EMISSION, RGB15(31,31,31));
	
	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	
	//ds specific, several attributes can be set here	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE );
	
	for(int x=0; x<32; x++)
	{
		for(int y=0; y<32; y++)
		{
			points[x][y][0]= (inttov16(x)/4);
			points[x][y][1]= (inttov16(y)/4);
			points[x][y][2]= sinLerp(x * (DEGREES_IN_CIRCLE / 32));
		}
	}
	
	return;
}
int main()
{	
	InitGL();
	
	glMatrixMode(GL_MODELVIEW);
	
	while (1) 
	{
		DrawGLScene();
		
		// flush to screen	
		glFlush(0);
		
		// wait for the screen to refresh
		swiWaitForVBlank();
	}
	
	return 0;
}

int DrawGLScene()											// Here's Where We Do All The Drawing
{
	int x, y;
	t16 float_x, float_y, float_xb, float_yb;
	
	glColor3b(255,255,255);    // set the vertex color
	
	glLoadIdentity();									// Reset The View

	glTranslatef(0.0f,0.0f,-12.0f);
	  
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);  
	glRotatef(zrot,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	for( x = 0; x < 31; x++ )
	{
		for( y = 0; y < 31; y++ )
		{
			float_x = inttot16(x) << 2;
			float_y = inttot16(y) << 2;
			float_xb = inttot16(x+1) << 2;
			float_yb = inttot16(y+1) << 2;

			glTexCoord2t16( float_x, float_y);
			glVertex3v16( points[x][y][0], points[x][y][1], points[x][y][2] );

			glTexCoord2t16( float_x, float_yb );
			glVertex3v16( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

			glTexCoord2t16( float_xb, float_yb );
			glVertex3v16( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

			glTexCoord2t16( float_xb, float_y );
			glVertex3v16( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();

	if( wiggle_count == 2 )
	{
		for( y = 0; y < 32; y++ )
		{
			hold=points[0][y][2];
			for( x = 0; x < 32; x++)
			{
				points[x][y][2] = points[x+1][y][2];
			}
			points[31][y][2]=hold;
		}
		wiggle_count = 0;
	}

	wiggle_count++;

	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.4f;

	return TRUE;											// Everything Went OK

}
