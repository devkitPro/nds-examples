#include <nds.h>


//NB: This would look better if the object had a bit of texturing too (eyes, nose etc)


#include "statue_bin.h"


static void get_pen_delta( int *dx, int *dy )
{
	static int prev_pen[2] = { 0x7FFFFFFF, 0x7FFFFFFF };

	u32 keys = keysHeld();

	if( keys & KEY_TOUCH )
	{
		touchPosition touchXY=touchReadXY();

		if( prev_pen[0] != 0x7FFFFFFF )
		{
			*dx = (prev_pen[0] - touchXY.x);
			*dy = (prev_pen[1] - touchXY.y);
		}

		prev_pen[0] = touchXY.x;
		prev_pen[1] = touchXY.y;
	}
	else
	{
		prev_pen[0] = prev_pen[1] = 0x7FFFFFFF;
		*dx = *dy = 0;
	}
}



int main()
{

	int rotateX = 0;
	int rotateY = 0;

	powerON(POWER_ALL);

	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);

	//irqs are nice
	irqInit();
	irqSet(IRQ_VBLANK, 0);

	//this should work the same as the normal gl call
	glViewPort(0,0,255,191);


	glClearColor(0,0,0);
	glClearDepth(0x7FFF);


	//toon-table entry 0 is for completely unlit pixels, going up to entry 31 for completely lit
	//We block-fill it in two halves, we get cartoony 2-tone lighting
	glSetToonTableRange( 0, 15, RGB15(8,8,8) );
	glSetToonTableRange( 16, 31, RGB15(24,24,24) );

	while(1)
	{
		glReset();

		//any floating point gl call is being converted to fixed prior to being implemented
		gluPerspective(35, 256.0 / 192.0, 0.1, 40);



		//NB: When toon-shading, the hw ignores lights 2 and 3
		//Also note that the hw uses the RED component of the lit vertex to index the toon-table
		glLight(0, RGB15(16,16,16) , 0,		floatov10(-1.0),		0);
		glLight(1, RGB15(16,16,16),   floatov10(-1.0),	0,		0);



		glMatrixMode(GL_MODELVIEW);
		glTranslate3f32(0, 0, floatof32(-3));
		glRotateXi(rotateX>>3);
		glRotateYi(rotateY>>3);


		glMaterialf(GL_AMBIENT, RGB15(8,8,8));
		glMaterialf(GL_DIFFUSE, RGB15(24,24,24));
		glMaterialf(GL_SPECULAR, RGB15(0,0,0));
		glMaterialf(GL_EMISSION, RGB15(0,0,0));

		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | POLY_TOON_SHADING);


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


		glCallList((u32*)statue_bin);

		glFlush();

		//swi seems to be broken, will let you know when i get this POS figured out
		//swiWaitForVBlank();
	}

	return 0;
}//end main
