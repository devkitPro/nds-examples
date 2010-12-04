/****************************************
 * 		NDS NeHe Lesson 10    			*
 * 		Author: Dovoto					*
 ****************************************/

// include your ndslib
#include <nds.h>
#include <malloc.h>
#include <stdio.h>

//needed to load pcx files
#include <nds/arm9/image.h>
#include <nds/arm9/trig_lut.h>


#include "Mud_pcx.h"
#include "World_txt.h"
#include "drunkenlogo_pcx.h"


int DrawGLScene();

const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;

float yrot = 0;
float walkbias = 0;
float walkbiasangle = 0;
float lookupdown = 0.0f;

int	texture[2];

typedef struct tagVERTEX
{
	float x, y, z;
	float u, v;
} VERTEX;

typedef struct tagTRIANGLE
{
	VERTEX vertex[3];
} TRIANGLE;

typedef struct tagSECTOR
{
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;

  SECTOR sector1;				// Our Model Goes Here:


float sin(float angle)
{
	int32 s = sinLerp((short)(angle * DEGREES_IN_CIRCLE / 360));

	return f32tofloat(s);
}

float cos(float angle)
{
	int32 c = cosLerp((short)(angle * DEGREES_IN_CIRCLE / 360));

	return f32tofloat(c);
}
 u8* file = (u8*)World_txt;

 void myGetStr(char* buff, int size)
{
	*buff = *file++;

	while( (*buff != '\n') && (*buff != 0xD))
	{
		buff++;
		*buff = *file++;
	}
	
	buff[0] = '\n';	
	buff[1] = 0;
	
}


void readstr(char *string)
{
	do
	{
		myGetStr(string, 255);
	} while ((string[0] == '/') || (string[0] == '\n' ));
	return;
}

void SetupWorld()
{
	float x, y, z, u, v;
	int numtriangles;
	char oneline[255];

	readstr(oneline);
	sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);

	sector1.triangle = (TRIANGLE*)malloc(numtriangles*sizeof(TRIANGLE));
	sector1.numtriangles = numtriangles;

	for (int loop = 0; loop < numtriangles; loop++)
	{
		for (int vert = 0; vert < 3; vert++)
		{
			readstr(oneline);
			sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			sector1.triangle[loop].vertex[vert].x = x;
			sector1.triangle[loop].vertex[vert].y = y;
			sector1.triangle[loop].vertex[vert].z = z;
			sector1.triangle[loop].vertex[vert].u = u;
			sector1.triangle[loop].vertex[vert].v = v;
		}
	}
	return;
}
int LoadGLTextures()									// Load PCX files And Convert To Textures
{
	sImage pcx;                //////////////(NEW) and different from nehe.

	glGenTextures(2, &texture[0]);
	
	//load our texture
	loadPCX((u8*)Mud_pcx, &pcx);
	image8to16(&pcx);

	glBindTexture(0, texture[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, pcx.image.data8);
	
	imageDestroy(&pcx);

	loadPCX((u8*)drunkenlogo_pcx, &pcx);
	image8to16(&pcx);
	glBindTexture(0, texture[1]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, pcx.image.data8);
	
	return TRUE;
}

struct CubeRot
{
	float x,y,z;
	CubeRot()
		: x(0), y(0), z(0)
	{
	}
} cubeRot;

void TransformCube()
{
	glRotatef(cubeRot.x,1.0f,0.0f,0.0f);
	glRotatef(cubeRot.y,0.0f,1.0f,0.0f);
	glRotatef(cubeRot.z,0.0f,0.0f,1.0f);
}

void EmitCube()
{
	glPushMatrix();
	glScalef(0.03f,0.03f,0.03f);	
	
	glRotatef(cubeRot.x,1.0f,0.0f,0.0f);
	glRotatef(cubeRot.y,0.0f,1.0f,0.0f);
	glRotatef(cubeRot.z,0.0f,0.0f,1.0f);
		
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
	glPopMatrix(1);
}

void ShadowDemo()
{
	cubeRot.y+=0.8f;
	
	//draw the actual cube
	glPushMatrix();
	glTranslatef(0.0f,0.4f,-0.4f); //draw the cube up in the air
	TransformCube();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	EmitCube();
	glPopMatrix(1);

	//draw the shadow:
	{
		//draw the cube shadow on the ground
		glPushMatrix();
		glTranslatef(0.0f,0.0f,-0.4f);
		TransformCube();
		
		//use no texture. set shadow color: we'll use green just to show that it is possible
		glBindTexture(0,0);
		glColor(RGB15(0,8,0));
		
		//1st shadow pass
		//be sure to use opaque here
		glPolyFmt(POLY_SHADOW | POLY_CULL_FRONT | POLY_ALPHA(31) );
		EmitCube();
		
		//2nd shadow pass
		//be sure to use a different polyID here (shadow with polyID 0 can't be cast on surface with polyID 0)
		//we set the fog bit here because we want the shadow to be fogged later. i think it may be buggy but it looks better.
		glPolyFmt(POLY_SHADOW | POLY_CULL_BACK | POLY_ALPHA(15) | POLY_ID(1) | POLY_FOG);
		EmitCube();
		
		//reset poly attribute
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0 | POLY_FOG);
		
		glPopMatrix(1);
	}
}

int main() {

	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE);                        //NEW  must set up some memory for textures
	
	consoleDemoInit();
	
	// initialize the geometry engine
	glInit();
	
	// enable textures
	glEnable(GL_TEXTURE_2D);
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// enable alpha blending for shadow demo
	glEnable(GL_BLEND);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	LoadGLTextures();
	SetupWorld();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);

	glLight(0, RGB15(31,31,31) , 0,				  floattov10(-1.0),		 0);
	
	//need to set up some material properties since DS does not have them set by default
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(16,16,16));

	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	
	//ds specific, several attributes can be set here	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0 | POLY_FOG);
	
	// Set the current matrix to be the model matrix
	glMatrixMode(GL_MODELVIEW);

	//setup demo fog parameters
	//these parameters are somewhat arbitrary, and designed to illustrate fog in just this one case.
	//you will certainly need to tweak them for your own use.
	//be sure to have set the POLY_FOG bit on any material you want to be fogged.
	glEnable(GL_FOG);
	glFogShift(2);
	glFogColor(0,0,0,0);
	for(int i=0;i<32;i++)
		glFogDensity(i,i*4);
	glFogDensity(31,127);
	glFogOffset(0x6000);

	while (1)
	{
		//these little button functions are pretty handy
		scanKeys();
				
	
		if (keysHeld() & KEY_A)
		{
			lookupdown -= 1.0f;
		}
		if (keysHeld() & KEY_B)
		{
			lookupdown += 1.0f;
		}
		if (keysHeld() & KEY_LEFT)
		{
			heading += 1.0f;	
			yrot = heading;
		}
		if (keysHeld() & KEY_RIGHT)
		{
			heading -= 1.0f;
			yrot = heading;
		}
		if (keysHeld() & KEY_DOWN)
		{
			
			xpos += (float)sin(heading) * 0.05f;
			zpos += (float)cos(heading) * 0.05f;

			if (walkbiasangle >= 359.0f)
			{
				walkbiasangle = 0.0f;
			}
			else
			{
				walkbiasangle+= 10;
			}
			
			walkbias = (float)sin(walkbiasangle)/20.0f;
		}
		if (keysHeld() & KEY_UP)
		{
			xpos -= (float)sin(heading) * 0.05f;
			zpos -= (float)cos(heading) * 0.05f;
			if (walkbiasangle <= 1.0f)
			{
				walkbiasangle = 359.0f;
			}
			else
			{
				walkbiasangle-= 10;
			}
			walkbias = (float)sin(walkbiasangle)/20.0f;
		}

		glColor3f(1,1,1);
		
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
											// Reset The View

	float x_m, y_m, z_m, u_m, v_m;
	float xtrans = -xpos;
	float ztrans = -zpos;
	float ytrans = -walkbias-0.25f;
	float sceneroty = 360.0f - yrot;
	
	glLoadIdentity();
	
	int numtriangles;

	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);
	
	glTranslatef(xtrans, ytrans, ztrans);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	numtriangles = sector1.numtriangles;
	
	
	// Process Each Triangle
	for (int loop_m = 0; loop_m < numtriangles; loop_m++)
	{
		glBegin(GL_TRIANGLES);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			x_m = sector1.triangle[loop_m].vertex[0].x;
			y_m = sector1.triangle[loop_m].vertex[0].y;
			z_m = sector1.triangle[loop_m].vertex[0].z;
			u_m = sector1.triangle[loop_m].vertex[0].u;
			v_m = sector1.triangle[loop_m].vertex[0].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
			
			x_m = sector1.triangle[loop_m].vertex[1].x;
			y_m = sector1.triangle[loop_m].vertex[1].y;
			z_m = sector1.triangle[loop_m].vertex[1].z;
			u_m = sector1.triangle[loop_m].vertex[1].u;
			v_m = sector1.triangle[loop_m].vertex[1].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
			
			x_m = sector1.triangle[loop_m].vertex[2].x;
			y_m = sector1.triangle[loop_m].vertex[2].y;
			z_m = sector1.triangle[loop_m].vertex[2].z;
			u_m = sector1.triangle[loop_m].vertex[2].u;
			v_m = sector1.triangle[loop_m].vertex[2].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
		glEnd();
	}

	ShadowDemo();
	
	return TRUE;										// Everything Went OK

}
