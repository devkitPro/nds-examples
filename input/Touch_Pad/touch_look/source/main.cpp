/********************************************************************************************
 * 		Nehe lesson 10 modification which uses the touch screen to control the camera and dpad
 *		to move the player.

 * 		Author: revo																		*
 *		Updated by revo (from 10b) - added camera moving by touching touch screen
 *
 ********************************************************************************************/

#include <nds.h>
#include <malloc.h>
#include <stdio.h>

#include "Mud_pcx.h"
#include "World_txt.h"


int DrawGLScene();

int heading;
int32 xpos;
int32 zpos;

int	yrot;				// Y Rotation
int32 walkbias = 0;
int walkbiasangle = 0;
int lookupdown = 0;


int	texture[1];			// Storage For 1 Textures (only going to use 1 on the DS for this demo)

typedef struct tagVERTEX {
	v16 x, y, z;
	t16 u, v;
} VERTEX;

typedef struct tagTRIANGLE {
	VERTEX vertex[3];
} TRIANGLE;

typedef struct tagSECTOR {
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;

SECTOR sector1;				// Our Model Goes Here:



u8* file = (u8*)World_txt;

void myGetStr(char* buff, int size) {
	*buff = *file++;

	while( (*buff != '\n') && (*buff != 0xD))
	{
		buff++;
		*buff = *file++;
	}

	buff[0] = '\n';
	buff[1] = 0;

}


void readstr(char *string) {
	do {
		myGetStr(string, 255);
	} while ((string[0] == '/') || (string[0] == '\n' ));
}

void SetupWorld() {
	float x, y, z;
	float u, v;
	int numtriangles;
	char oneline[255];

	readstr(oneline);
	sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);

	sector1.triangle = (TRIANGLE*)malloc(numtriangles*sizeof(TRIANGLE));
	sector1.numtriangles = numtriangles;

	for (int loop = 0; loop < numtriangles; loop++) {
		for (int vert = 0; vert < 3; vert++) {
			readstr(oneline);
			sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			sector1.triangle[loop].vertex[vert].x = floattov16(x);
			sector1.triangle[loop].vertex[vert].y = floattov16(y);
			sector1.triangle[loop].vertex[vert].z = floattov16(z);
			sector1.triangle[loop].vertex[vert].u = floattot16(u*128);
			sector1.triangle[loop].vertex[vert].v = floattot16(v*128);
		}
	}

	return;
}

// Load PCX files And Convert To Textures
int LoadGLTextures() {
	sImage pcx;

	//load our texture
	loadPCX((u8*)Mud_pcx, &pcx);

	image8to16(&pcx);

	glGenTextures(1, &texture[0]);
	glBindTexture(0, texture[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, pcx.image.data8);

	imageDestroy(&pcx);

	return TRUE;
}

int main() {

	// Setup the Main screen for 3D
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE);                        //NEW  must set up some memory for textures
	
	// initialize the 3D engine
	glInit();
	
	// enable textures
	glEnable(GL_TEXTURE_2D);
	
	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	// setup the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
	
	glLight(0, RGB15(31,31,31) , 0,	floattov10(-1.0), 0);
	
	//need to set up some material properties since DS does not have them set by default
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(16,16,16));
	
	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	
	//ds specific, several attributes can be set here
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0);
	
	// Set the current matrix to be the model matrix
	glMatrixMode(GL_MODELVIEW);
	
	// Specify the Clear Color and Depth
	glClearColor(0,0,0,31);
	glClearDepth(0x7FFF);
	
	// set the vertex color to white
	glColor3f(1.0,1.0,1.0);
	
	LoadGLTextures();
	SetupWorld();

	touchPosition	thisXY;
	touchPosition	lastXY = { 0,0,0,0 };		

	while (1)
	{
		//these little button functions are pretty handy
		scanKeys();

		if (keysHeld() & (KEY_LEFT|KEY_Y))
		{
			xpos -= sinLerp(heading + degreesToAngle(90)) >> 5;
			zpos += cosLerp(heading + degreesToAngle(90)) >> 5;
		}
		if (keysHeld() & (KEY_RIGHT|KEY_A))
		{
			xpos += sinLerp(heading + degreesToAngle(90)) >> 5;
			zpos -= cosLerp(heading + degreesToAngle(90)) >> 5;
		}
		if (keysHeld() & (KEY_DOWN|KEY_B))
		{

			xpos -= sinLerp(heading)>>5;
			zpos += cosLerp(heading)>>5;

			walkbiasangle += degreesToAngle(5);

			walkbias = sinLerp(walkbiasangle)>>4;
		}
		if (keysHeld() & (KEY_UP|KEY_X))
		{
			xpos += sinLerp(heading) >> 5;
			zpos -= cosLerp(heading) >> 5;

			if (walkbiasangle <= 0)
			{
				walkbiasangle = DEGREES_IN_CIRCLE;
			}
			else
			{
				walkbiasangle -= degreesToAngle(5);
			}
			walkbias = sinLerp(walkbiasangle)>>4;
		}

		// Camera rotation by touch screen

		if (keysHeld() & KEY_TOUCH)
		{
			touchRead(&thisXY);

			int16 dx = thisXY.px - lastXY.px;
			int16 dy = thisXY.py - lastXY.py;

			// filtering measurement errors
			if (dx<20 && dx>-20 && dy<20 && dy>-20)
			{
				if(dx>-3&&dx<3)
					dx=0;

				if(dy>-2&&dy<2)
					dy=0;

					lookupdown -= degreesToAngle(dy);

					heading += degreesToAngle(dx);
					yrot = heading;
			}

			lastXY = thisXY;
		}


		//Push our original Matrix onto the stack (save state)
		glPushMatrix();

		DrawGLScene();

		// Pop our Matrix from the stack (restore state)
		glPopMatrix(1);

		// flush to screen
		glFlush(0);

	}

	return 0;
}

int DrawGLScene()											// Here's Where We Do All The Drawing
{
											// Reset The View

	v16 x_m, y_m, z_m;
	t16 u_m, v_m;
	int32 xtrans = -xpos;
	int32 ztrans = -zpos;
	int32 ytrans = -walkbias-(1<<10);
	int sceneroty = DEGREES_IN_CIRCLE - yrot;

	glLoadIdentity();

	int numtriangles;

	glRotatef32i(lookupdown,(1<<12),0,0);
	glRotatef32i(sceneroty,0,(1<<12),0);

	glTranslatef32(xtrans, ytrans, ztrans);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	numtriangles = sector1.numtriangles;


	// Process Each Triangle
	for (int loop_m = 0; loop_m < numtriangles; loop_m++)
	{
		glBegin(GL_TRIANGLES);
			glNormal(NORMAL_PACK( 0, 0, 1<<10));
			x_m = sector1.triangle[loop_m].vertex[0].x;
			y_m = sector1.triangle[loop_m].vertex[0].y;
			z_m = sector1.triangle[loop_m].vertex[0].z;
			u_m = sector1.triangle[loop_m].vertex[0].u;
			v_m = sector1.triangle[loop_m].vertex[0].v;
			glTexCoord2t16(u_m,v_m); glVertex3v16(x_m,y_m,z_m);

			x_m = sector1.triangle[loop_m].vertex[1].x;
			y_m = sector1.triangle[loop_m].vertex[1].y;
			z_m = sector1.triangle[loop_m].vertex[1].z;
			u_m = sector1.triangle[loop_m].vertex[1].u;
			v_m = sector1.triangle[loop_m].vertex[1].v;
			glTexCoord2t16(u_m,v_m); glVertex3v16(x_m,y_m,z_m);

			x_m = sector1.triangle[loop_m].vertex[2].x;
			y_m = sector1.triangle[loop_m].vertex[2].y;
			z_m = sector1.triangle[loop_m].vertex[2].z;
			u_m = sector1.triangle[loop_m].vertex[2].u;
			v_m = sector1.triangle[loop_m].vertex[2].v;
			glTexCoord2t16(u_m,v_m); glVertex3v16(x_m,y_m,z_m);
		glEnd();
	}
	return TRUE;										// Everything Went OK

}
