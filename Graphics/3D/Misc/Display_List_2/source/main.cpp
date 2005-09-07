#include <nds.h>

//teapot display list provided by Mike260, as well as the display list gl code.
#include "teapot_bin.h"


int main()
{	

	float rotateX = 0.0;
	float rotateY = 0.0;

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
	
	
	
	while(1)		
	{
		glReset();
	
		//any floating point gl call is being converted to fixed prior to being implemented
		gluPerspective(35, 256.0 / 192.0, 0.1, 40);

		gluLookAt(	0.0, 0.0, 1.0,		//camera possition 
					0.0, 0.0, 0.0,		//look at
					0.0, 1.0, 0.0);		//up
		
		

		
		glLight(0, RGB15(31,31,31) , 0,				  floatov10(-1.0),		 0);
		glLight(1, RGB15(31,0,31),   0,				  floatov10(1) - 1,			 0);
		glLight(2, RGB15(0,31,0) ,   floatov10(-1.0), 0,					 0);
		glLight(3, RGB15(0,0,31) ,   floatov10(1.0) - 1,  0,					 0);

		glPushMatrix();

		//move it away from the camera
		glTranslate3f32(0, 0, floatof32(-1));
				
		glRotateX(rotateX);
		glRotateY(rotateY);
		
		glMatrixMode(GL_TEXTURE);
		glIdentity();
		
		glMatrixMode(GL_MODELVIEW);

	
		//not a real gl function and will likely change
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 | 
													POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3 ) ;
		
		scanKeys();
		
		u16 keys = keysHeld();
		
		if(!(keys & KEY_UP)) rotateX += 3;
		if(!(keys & KEY_DOWN)) rotateX -= 3;
		if(!(keys & KEY_LEFT)) rotateY += 3;
		if(!(keys & KEY_RIGHT)) rotateY -= 3;
		
		glCallList((u32*)teapot_bin);	

		glPopMatrix(1);
			
		glFlush();

		swiWaitForVBlank();
	}

	return 0;
}//end main 
