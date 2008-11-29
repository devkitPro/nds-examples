/****************************************
 * 		NDS NeHe Lesson 09    			*
 * 		Author: dovoto
 *		DS does not appear to support 
		the features needed for this demo
 ****************************************/

// include your ndslib
#include <nds.h>
#include <malloc.h>
//needed to load pcx files
#include <nds/arm9/image.h>
#include <stdlib.h>

#include "Star_pcx.h"

int DrawGLScene();


bool	twinkle;		// Twinkling Stars
bool	tp;				// 'T' Key Pressed?

const	int num=50;		// Number Of Stars To Draw

typedef struct			// Create A Structure For Star
{
	int r, g, b;		// Stars Color
	float dist,		// Stars Distance From Center
			angle;		// Stars Current Angle
}stars;

stars star[num];		// Need To Keep Track Of 'num' Stars

float	zoom=-15.0f;	// Distance Away From Stars
float tilt=90.0f;		// Tilt The View
float	spin;			// Spin Stars

int	loop;			// General Loop Variable
int	texture[1];			// Storage For One textures


// Load PCX files And Convert To Textures
int LoadGLTextures() {
	sImage pcx;			// (NEW) and different from nehe.

	//load our texture
	loadPCX((u8*)Star_pcx, &pcx);
	image8to16trans(&pcx, 0);

	//DS supports no filtering of anykind so no need for more than one texture
	glGenTextures(1, &texture[0]);
	glBindTexture(0, texture[0]);
	glTexImage2D(0, 0, GL_RGBA, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, pcx.image.data8);

	imageDestroy(&pcx);

	return TRUE;
}

int main() {
	
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE);                        //NEW  must set up some memory for textures
	
	// initialize the geometry engine
	glInit();
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	// enable textures
	glEnable(GL_TEXTURE_2D);
	
	// enable alpha blending
	glEnable(GL_BLEND);

	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	LoadGLTextures();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
	glColor3f(1,1,1);
	
	//set up a directional ligth arguments are light number (0-3), light color, 
	//and an x,y,z vector that points in the direction of the light
	glLight(0, RGB15(31,31,31) , 0, 0,floattov10(-1.0));
	
	//need to set up some material properties since DS does not have them set by default
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(16,16,16));
	
	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	
	glPolyFmt(POLY_ALPHA(15) | POLY_CULL_BACK  | POLY_FORMAT_LIGHT0);

	glMatrixMode(GL_MODELVIEW);
	
	while (1) {
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
	glBindTexture(GL_TEXTURE_2D, texture[0]);			// Select Our Texture

	for (loop=0; loop<num; loop++)						// Loop Through All The Stars
	{
		glLoadIdentity();								// Reset The View Before We Draw Each Star
		glTranslatef(0.0f,0.0f,zoom);					// Zoom Into The Screen (Using The Value In 'zoom')
		glRotatef(tilt,1.0f,0.0f,0.0f);					// Tilt The View (Using The Value In 'tilt')
		glRotatef(star[loop].angle,0.0f,1.0f,0.0f);		// Rotate To The Current Stars Angle
		glTranslatef(star[loop].dist,0.0f,0.0f);		// Move Forward On The X Plane
		glRotatef(-star[loop].angle,0.0f,1.0f,0.0f);	// Cancel The Current Stars Angle
		glRotatef(-tilt,1.0f,0.0f,0.0f);				// Cancel The Screen Tilt
		
		if (twinkle)
		{
			glColor3b(star[(num-loop)-1].r,star[(num-loop)-1].g,star[(num-loop)-1].b);  ///different
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
		}

		glRotatef(spin,0.0f,0.0f,1.0f);
		glColor3b(star[loop].r,star[loop].g,star[loop].b);                            //different
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();

		spin+=0.01f;
		star[loop].angle+=float(loop)/num;
		star[loop].dist-=0.01f;
		if (star[loop].dist<0.0f)
		{
			star[loop].dist+=5.0f;
			star[loop].r=rand()%256;
			star[loop].g=rand()%256;
			star[loop].b=rand()%256;
		}
	}
	return TRUE;															// Keep Going

}
