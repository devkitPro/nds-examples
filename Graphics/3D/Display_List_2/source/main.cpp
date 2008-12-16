#include <nds.h>

//teapot display list provided by Mike260, as well as the display list gl code.
#include "teapot_bin.h"


int main() {

	float rotateX = 0.0;
	float rotateY = 0.0;

	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);

	// initialize gl
	glInit();
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call
	glViewport(0,0,255,191);
	
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	gluLookAt(	0.0, 0.0, 3.5,		//camera possition 
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
	
	glLight(0, RGB15(31,31,31) , 0,				  floattov10(-1.0),		 0);
	glLight(1, RGB15(31,0,31),   0,				  floattov10(1) - 1,			 0);
	glLight(2, RGB15(0,31,0) ,   floattov10(-1.0), 0,					 0);
	glLight(3, RGB15(0,0,31) ,   floattov10(1.0) - 1,  0,					 0);
	
	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | 
			  POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3 ) ;
	
	while(1)		
	{
		glPushMatrix();
				
		glRotateX(rotateX);
		glRotateY(rotateY);
		
		scanKeys();
		u16 keys = keysHeld();
		if(!(keys & KEY_UP)) rotateX += 3;
		if(!(keys & KEY_DOWN)) rotateX -= 3;
		if(!(keys & KEY_LEFT)) rotateY += 3;
		if(!(keys & KEY_RIGHT)) rotateY -= 3;
		
		glCallList((u32*)teapot_bin);	

		glPopMatrix(1);
			
		glFlush(0);
	}

	return 0;
}//end main 
