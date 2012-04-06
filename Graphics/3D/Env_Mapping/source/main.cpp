#include <nds.h>


#include "teapot_bin.h"
#include "cafe_bin.h"


static void get_pen_delta( int *dx, int *dy ) {

	static int prev_pen[2] = { 0x7FFFFFFF, 0x7FFFFFFF };
	touchPosition touchXY;
	
	u32 keys = keysHeld();

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

	// intialize gl
	glInit();
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call
	glViewport(0,0,255,191);

	vramSetBankA(VRAM_A_TEXTURE);
	glEnable(GL_TEXTURE_2D);
	
	int cafe_texid;
	glGenTextures( 1, &cafe_texid );
	glBindTexture( 0, cafe_texid );
	glTexImage2D( 0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_NORMAL, (u8*)cafe_bin );

	
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	while(1) {
		//TEXGEN_NORMAL helpfully pops our normals into this matrix and uses the result as texcoords
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		GLvector tex_scale = { 64<<16, -64<<16, 1<<16 };
		glScalev( &tex_scale );		//scale normals up from (-1,1) range into texcoords
		glRotateXi(rotateX);		//rotate texture-matrix to match the camera
		glRotateYi(rotateY);


		glMatrixMode(GL_POSITION);
		glLoadIdentity();
		glTranslatef32(0, 0, floattof32(-3));
		glRotateXi(rotateX);
		glRotateYi(rotateY);

		glMaterialf(GL_EMISSION, RGB15(31,31,31));

		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK );

		scanKeys();
		u32 keys = keysHeld();

		if( keys & KEY_UP ) rotateX += 3<<3;
		if( keys & KEY_DOWN ) rotateX -= 3<<3;
		if( keys & KEY_LEFT ) rotateY += 3<<3;
		if( keys & KEY_RIGHT ) rotateY -= 3<<3;

		int pen_delta[2];
		get_pen_delta( &pen_delta[0], &pen_delta[1] );
		rotateY -= pen_delta[0];
		rotateX -= pen_delta[1];


		glBindTexture( 0, cafe_texid );
		glCallList((u32*)teapot_bin);

		glFlush(0);
	}

	return 0;
}//end main
