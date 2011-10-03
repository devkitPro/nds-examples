#include <nds.h>


//NB: This would look better if the object had a bit of texturing too (eyes, nose etc)


#include "statue_bin.h"


static void get_pen_delta( int *dx, int *dy ) {
	static int prev_pen[2] = { 0x7FFFFFFF, 0x7FFFFFFF };

	u32 keys = keysHeld();

	touchPosition touchXY;

	if( keys & KEY_TOUCH ) {
		
		touchRead(&touchXY);
		if( prev_pen[0] != 0x7FFFFFFF ) {
			*dx = (prev_pen[0] - touchXY.rawx);
			*dy = (prev_pen[1] - touchXY.rawy);
		} else {
			*dx = *dy = 0;
		}

		prev_pen[0] = touchXY.rawx;
		prev_pen[1] = touchXY.rawy;
	} else {
		prev_pen[0] = prev_pen[1] = 0x7FFFFFFF;
		*dx = *dy = 0;
	}
}



int main() {

	int rotateX = 0;
	int rotateY = 0;

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

	//toon-table entry 0 is for completely unlit pixels, going up to entry 31 for completely lit
	//We block-fill it in two halves, we get cartoony 2-tone lighting
	glSetToonTableRange( 0, 15, RGB15(8,8,8) );
	glSetToonTableRange( 16, 31, RGB15(24,24,24) );
	
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	//NB: When toon-shading, the hw ignores lights 2 and 3
	//Also note that the hw uses the RED component of the lit vertex to index the toon-table
	glLight(0, RGB15(16,16,16) , 0,		floattov10(-1.0),		0);
	glLight(1, RGB15(16,16,16),   floattov10(-1.0),	0,		0);
	
	gluLookAt(	0.0, 0.0, -3.0,		//camera possition 
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
	
	while(1) {

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glRotateXi(rotateX);
			glRotateYi(rotateY);


			glMaterialf(GL_AMBIENT, RGB15(8,8,8));
			glMaterialf(GL_DIFFUSE, RGB15(24,24,24));
			glMaterialf(GL_SPECULAR, RGB15(0,0,0));
			glMaterialf(GL_EMISSION, RGB15(0,0,0));

			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_TOON_HIGHLIGHT);


			scanKeys();
			u32 keys = keysHeld();

			if( keys & KEY_UP ) rotateX += 1;
			if( keys & KEY_DOWN ) rotateX -= 1;
			if( keys & KEY_LEFT ) rotateY += 1;
			if( keys & KEY_RIGHT ) rotateY -= 1;

			int pen_delta[2];
			get_pen_delta( &pen_delta[0], &pen_delta[1] );
			rotateY -= pen_delta[0];
			rotateX -= pen_delta[1];

			glCallList((u32*)statue_bin);
			glPopMatrix(1);

		glFlush(0);

		swiWaitForVBlank();
	}

	return 0;
}
