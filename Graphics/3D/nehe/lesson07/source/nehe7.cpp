/****************************************
 * 		NDS NeHe Lesson 07    			*
 * 		Author: Ethos					*
 ****************************************/

// include your ndslib
#include <nds.h>
#include <malloc.h>
//needed to load pcx files
#include <nds/arm9/image.h>


#include "drunkenlogo_pcx.h"

int DrawGLScene();
bool	light;				// Lighting ON/OFF ( NEW )
bool	lp;					// L Pressed? ( NEW )

float	xrot;				// X Rotation
float	yrot;				// Y Rotation
float xspeed;				// X Rotation Speed
float yspeed;				// Y Rotation Speed
float	z=-5.0f;			// Depth Into The Screen

float LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
float LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
float LightPosition[]=	{ 0.0f, 0.0f, 2.0f, 1.0f };

int	texture[3];			// Storage For 3 Textures (only going to use 1 on the DS for this demo)



int LoadGLTextures()									// Load PCX files And Convert To Textures
{
	sImage pcx;                //////////////(NEW) and different from nehe.

	//load our texture
	loadPCX((u8*)drunkenlogo_pcx, &pcx);
	image8to16(&pcx);

	//DS supports no filtering of anykind so no need for more than one texture
	glGenTextures(1, &texture[0]);
	glBindTexture(0, texture[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, pcx.image.data8);

	imageDestroy(&pcx);

	return TRUE;
}

int main() {
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE);                        //NEW  must set up some memory for textures
	
	// initialize the geometry engine
	glInit();
	
	// enable textures
	glEnable(GL_TEXTURE_2D);
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	LoadGLTextures();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
	
	//set up a directional light arguments are light number (0-3), light color, 
	//and an x,y,z vector that points in the direction of the light, the direction must be normalized
	glLight(0, RGB15(31,31,31) , 0, floattov10(-1.0), 0);
	
	//need to set up some material properties since DS does not have them set by default
	glMaterialf(GL_AMBIENT, RGB15(8,8,8));
	glMaterialf(GL_DIFFUSE, RGB15(8,8,8));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(16,16,16));
	
	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	
	// Set the current matrix to be the model matrix
	glMatrixMode(GL_MODELVIEW);
	
	while (1) 
	{
		//these little button functions are pretty handy
		scanKeys();

	
		if ((keysDown() & KEY_A)) light = !light;
		if (keysHeld() & KEY_R) z-=0.02f;
		if (keysHeld() & KEY_L) z+=0.02f;
		if (keysHeld() & KEY_LEFT) xspeed-=0.01f;
		if (keysHeld() & KEY_RIGHT) xspeed+=0.01f;
		if (keysHeld() & KEY_UP) yspeed+=0.01f;
		if (keysHeld() & KEY_DOWN) yspeed-=0.01f;
		
		
		glColor3f(1,1,1);
		
		if (!light)
		{
			//ds specific, several attributes can be set here including turning on our light	
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);
		}
		else
		{
			//ds specific, several attributes can be set here including turning on our light	
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK  | POLY_FORMAT_LIGHT0);
		}

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
	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,z);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);  //no filters to swtich between

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();

	xrot+=xspeed;
	yrot+=yspeed;
	return TRUE;										// Keep Going

}
