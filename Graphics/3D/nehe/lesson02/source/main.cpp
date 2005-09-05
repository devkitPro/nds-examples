/****************************************
 * 		NDS NeHe Lesson 02    			*
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
	
	// IRQ basic setup
	irqInit();
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

int DrawGLScene()											// Here's Where We Do All The Drawing
{
	
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(-1.5f,0.0f,-6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
		glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
	glEnd();											// Finished Drawing The Triangle
	glTranslatef(3.0f,0.0f,0.0f);						// Move Right 3 Units
	glBegin(GL_QUADS);									// Draw A Quad
		glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
		glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
		glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
	glEnd();											// Done Drawing The Quad
	return TRUE;										// Keep Going
}
