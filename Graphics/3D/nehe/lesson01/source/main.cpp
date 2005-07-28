/****************************************
 * 		NDS NeHe Lesson 01    			*
 * 		Author: Dovoto					*
 ****************************************/

// include your ndslib
#include <nds.h>


int DrawGLScene();

int main()
{	
	// Turn on everything
	powerON(POWER_ALL);
	
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	
	// IRQ basic setup (not strickly required but nice
	irqInitHandler(irqDefaultHandler);
	irqSet(IRQ_VBLANK, 0);

	
	// Set our viewport to be the same size as the screen
	glViewPort(0,0,255,191);
	
	// Specify the Clear Color and Depth 
	glClearColor(0,0,0);

	glClearDepth(0x7FFF);
	
	while (1) 
	{
		// Reset the screen and setup the view
		glReset();
		gluPerspective(35, 256.0 / 192.0, 0.1, 100);
		glColor3f(1, 1, 1);									// Set the color..not in nehe source...ds gl default will be black

		//ds specific, several attributes can be set here	
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
		
		// Set the current matrix to be the model matrix
		glMatrixMode(GL_MODELVIEW);
		
		//Push our original Matrix onto the stack (save state)
		glPushMatrix();	

		DrawGLScene();
		
		// Pop our Matrix from the stack (restore state)
		glPopMatrix(1);

		//a handy little built in function to wait for a screen refresh
		swiWaitForVBlank();

		// flush to screen	
		glFlush();
	
	}
	
	return 0;
}

int DrawGLScene(void)
{
	//we are going to use floating point for the tutorial...keep in mind the DS has no
	//floating point hardware.  For real life use the built in fixed point types.
	//this is where the magic happens
	glLoadIdentity();
	return TRUE;
}
