/*---------------------------------------------------------------------------------

	$Id: main.c,v 1.5 2007-01-11 05:33:41 dovoto Exp $
	
	Nehe lesson 10 modified to use gbfs
	
	$Log: not supported by cvs2svn $
	Revision 1.4  2005/11/26 21:26:59  joatski
	Changed to use corrected fixed-point macro names
	
	Revision 1.3  2005/09/05 00:32:19  wntrmute
	removed references to IPC struct
	replaced with API functions
	
	Revision 1.2  2005/08/31 03:02:39  wntrmute
	updated for new stdio support
	
	Revision 1.1  2005/08/05 02:34:21  wntrmute
	*** empty log message ***
	
 
---------------------------------------------------------------------------------*/

#include <nds.h>				// include your ndslib
#include <nds/arm9/image.h>		// needed to load pcx files
#include <nds/arm9/trig_lut.h>
#include <stdio.h>
#include <stdlib.h>

#include <gbfs.h>				// filesystem functions

extern const GBFS_FILE  data_gbfs;


int DrawGLScene();

const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;

GLfloat	yrot;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;

int	texture[3];			// Storage For 3 Textures (only going to use 1 on the DS for this demo)

//---------------------------------------------------------------------------------
typedef struct tagVERTEX {
//---------------------------------------------------------------------------------
	float x, y, z;
	float u, v;
} VERTEX;

//---------------------------------------------------------------------------------
typedef struct tagTRIANGLE {
//---------------------------------------------------------------------------------
	VERTEX vertex[3];
} TRIANGLE;

//---------------------------------------------------------------------------------
typedef struct tagSECTOR {
//---------------------------------------------------------------------------------
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;

SECTOR sector1;				// Our Model Goes Here:


//---------------------------------------------------------------------------------
float tsin(float angle) {
//---------------------------------------------------------------------------------
	f32 s = SIN[(int)((angle * LUT_SIZE) / 360.0) & LUT_MASK];

	return f32tofloat(s);
}

//---------------------------------------------------------------------------------
float tcos(float angle) {
//---------------------------------------------------------------------------------
	f32 c = COS[(int)((angle * LUT_SIZE) / 360.0) & LUT_MASK];

	return f32tofloat(c);
}

const char* file;

//---------------------------------------------------------------------------------
void myGetStr(char* buff, int size) {
//---------------------------------------------------------------------------------
	*buff = *file++;

	while( (*buff != '\n') && (*buff != 0xD)) {
		buff++;
		*buff = *file++;
	}
	
	buff[0] = '\n';	
	buff[1] = 0;
	
}


//---------------------------------------------------------------------------------
void readstr(char *string) {
//---------------------------------------------------------------------------------
	do {
		myGetStr(string, 255);
	} while ((string[0] == '/') || (string[0] == '\n' ));
	return;
}

//---------------------------------------------------------------------------------
void SetupWorld() {
//---------------------------------------------------------------------------------
	float x, y, z, u, v;
	int numtriangles;
	char oneline[255];
	int loop, vert;

	file = gbfs_get_obj(&data_gbfs, "World.txt", NULL); 

	readstr(oneline);
	sscanf(oneline, "NUMPOLYS %d\n", &numtriangles);

	sector1.triangle = (TRIANGLE*)malloc(numtriangles*sizeof(TRIANGLE));
	sector1.numtriangles = numtriangles;
	
	for (loop = 0; loop < numtriangles; loop++) {
		for (vert = 0; vert < 3; vert++) {
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

//---------------------------------------------------------------------------------
// Load PCX files And Convert To Textures
//---------------------------------------------------------------------------------
int LoadGLTextures() {
//---------------------------------------------------------------------------------

	sImage pcx;			// (NEW) and different from nehe.

	//load our texture
	const char *pcx_file = gbfs_get_obj(&data_gbfs, "Mud.pcx", NULL); 
	loadPCX((u8*)pcx_file, &pcx);
	
	image8to16(&pcx);

	glGenTextures(1, &texture[0]);
	glBindTexture(0, texture[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, pcx.data8);

	return TRUE;
}

int main()
{	
	
	
	// Turn on everything
	powerON(POWER_ALL);
	
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE);                        //NEW  must set up some memory for textures

	// IRQ basic setup
	irqInit();
	irqSet(IRQ_VBLANK, 0);

	// Set our viewport to be the same size as the screen
	glViewPort(0,0,255,191);
	
	// Specify the Clear Color and Depth 
	glClearColor(0,0,0);
	glClearDepth(0x7FFF);
	LoadGLTextures();
	SetupWorld();
	

	while (1) 
	{
		//these little button functions are pretty handy
		scanKeys();
				
	
		if (keysHeld() & KEY_A) lookupdown -= 1.0f;

		if (keysHeld() & KEY_B) lookupdown += 1.0f;

		if (keysHeld() & KEY_LEFT) {
			heading -= 1.0f;	
			yrot = heading;
		}
		
		if (keysHeld() & KEY_RIGHT) {
			heading += 1.0f;
			yrot = heading;
		}
		
		if (keysHeld() & KEY_DOWN) {
			xpos -= (float)tsin(heading) * 0.05f;
			zpos += (float)tcos(heading) * 0.05f;
			if (walkbiasangle >= 359.0f)
			{
				walkbiasangle = 0.0f;
			}
			else
			{
				walkbiasangle+= 10;
			}
			walkbias = (float)tsin(walkbiasangle)/20.0f;
		}
		if (keysHeld() & KEY_UP)
		{
			xpos += tsin(heading) * 0.05f;
			zpos -= tcos(heading) * 0.05f;

			if (walkbiasangle <= 1.0f) {
				walkbiasangle = 359.0f;
			} else {
				walkbiasangle-= 10;
			}
			walkbias = (float)tsin(walkbiasangle)/20.0f;
		}

		// Reset the screen and setup the view
		glReset();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(35, 256.0 / 192.0, 0.1, 100);
		glColor3f(1,1,1);
		
		glLight(0, RGB15(31,31,31), 0, floattov10(-1.0), 0);

		glPushMatrix();
		
		glMatrixMode(GL_TEXTURE);
		glIdentity();
		
		glMatrixMode(GL_MODELVIEW);

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
		
		//Push our original Matrix onto the stack (save state)
		glPushMatrix();	

		DrawGLScene();
		
		// Pop our Matrix from the stack (restore state)
		glPopMatrix(1);

		// flush to screen	
		glFlush();
	
	}
	
	return 0;
}

//---------------------------------------------------------------------------------
// Here's Where We Do All The Drawing
//---------------------------------------------------------------------------------
int DrawGLScene() {
//---------------------------------------------------------------------------------

	// Reset The View
	GLfloat x_m, y_m, z_m, u_m, v_m;
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -walkbias-0.25f;
	GLfloat sceneroty = 360.0f - yrot;
	
	glLoadIdentity();
	
	int numtriangles;

	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);
	
	glTranslatef(xtrans, ytrans, ztrans);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	numtriangles = sector1.numtriangles;
	
	int loop_m;	
	// Process Each Triangle
	for (loop_m = 0; loop_m < numtriangles; loop_m++) {
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
	return TRUE;										// Everything Went OK

}
