/*---------------------------------------------------------------------------------

	$id $
	
	Box test to demonstrate 3D bounding box es.  also shows the effect of culling and
	clipping on vertex usage
	$log $
*--------------------------------------------------------------------------------*/


#include <nds.h>
#include <nds/arm9/console.h>
#include <nds/arm9/boxtest.h>

#include <stdio.h>



//some code for profiling
u16 startTimer(int timer)
{

	TIMER_CR(timer) = 0;
	TIMER_DATA(0) = 0;
	TIMER_CR(timer) = TIMER_DIV_1;
	return TIMER_DATA(0);
}

#define getTimer(timer) (TIMER_DATA(timer))

//draws a box...same signature as boxTest
void DrawBox(float x, float y, float z, float height, float width, float depth)
{
	glBegin(GL_QUADS);
	//z  face
	glColor3f(1,0,0);
	glVertex3f(x , y , z );
	glVertex3f(x + width, y , z );
	glVertex3f(x + width, y + height, z );
	glVertex3f(x , y + height, z );

	//z + depth face
	glColor3f(1,0,1);
	glVertex3f(x , y , z + depth);
	glVertex3f(x , y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y , z + depth);


	//x  face
	glColor3f(1,1,0);
	glVertex3f(x , y , z );
	glVertex3f(x , y + height, z );
	glVertex3f(x , y + height, z + depth);
	glVertex3f(x , y , z + depth);

	//x + width face
	glColor3f(1,1,1);
	glVertex3f(x + width, y , z );
	glVertex3f(x + width, y , z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y + height, z );

	//y  face
	glColor3f(0,1,0);
	glVertex3f(x , y , z );
	glVertex3f(x , y , z + depth);
	glVertex3f(x + width, y , z + depth);
	glVertex3f(x + width, y , z );

	//y  + height face
	glColor3f(0,1,1);
	glVertex3f(x , y + height, z );
	glVertex3f(x + width, y + height, z );
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x , y + height, z + depth);

	glEnd();

}

//draw the clock
int main()
{	

	// Turn on everything
	powerON(POWER_ALL);

	//put 3D on top
	lcdMainOnTop();

	//setup the sub screen for basic printing
	consoleDemoInit();

	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);

	// IRQ basic setup
	irqInit();
	irqSet(IRQ_VBLANK, 0);

	// initialize gl
	glInit();
	
	// Set our view port to be the same size as the screen
	glViewPort(0,0,255,191);

	// Specify the Clear Color and Depth 
	glClearColor(0,0,0);
	glClearDepth(0x7FFF);

	//camera
	float rotX = 0, rotY = 0;
	float translate = -3;

	//some profiling code
	u16 time;

	//keep track of vertex ram usage
	int polygon_count;
	int vertex_count;

	//object 
	int rx = 0, ry = 0;
	int oldx = 0, oldy = 0;

	//main loop
	while (1) 
	{

		scanKeys();

		//process input
		if(keysHeld() & KEY_LEFT) rotY--;
		if(keysHeld() & KEY_RIGHT) rotY++;
		if(keysHeld() & KEY_UP) rotX ++;
		if(keysHeld() & KEY_DOWN) rotX --;
		if(keysHeld() & KEY_L) translate += .1;
		if(keysHeld() & KEY_R) translate -= .1;

		//reset x and y when user touches screen
		if(keysDown() & KEY_TOUCH) 
		{
			oldx = touchReadXY().px;
			oldy = touchReadXY().py;
		}

		//if user drags then grab the delta
		if(keysHeld() & KEY_TOUCH) 
		{
			rx += touchReadXY().px - oldx; 
			ry += touchReadXY().py - oldy;
			oldx = touchReadXY().px;
			oldy = touchReadXY().py;
		}

		
		//change ortho vs perspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(keysHeld() & KEY_B)
			glOrtho(-2,2,-2,2,0.1,10);	
		else {
			gluPerspective(45, 256.0 / 192.0, 0.1, 10);
		}
		//change cull mode
		if(keysHeld() & KEY_A)
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE );
		else
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT );

		// Set the current matrix to be the model matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//handle camera
		glRotateY(rotY);
		glRotateX(rotX);
		glTranslatef(0,0,translate);

		//move the cube
		glRotateX(ry);
		glRotateY(rx);

		DrawBox(-1,-1,-1,2,2,2);

		printf("\x1b[2JBox test cycle count");

		time = startTimer(0);
		int hit = BoxTestf(-1,-1,-1,2,2,2);
		printf("\nSingle test (float): %i", 2*(getTimer(0) - time));

		time = startTimer(0);
		BoxTest(inttov16(-1),inttov16(-1),inttov16(-1),inttov16(2),inttov16(2),inttov16(2));
		printf("\nSingle test (fixed): %i", 2*(getTimer(0) - time));

		time = startTimer(0);
		for(int i = 0; i < 64; i++)
			BoxTest(inttov16(-1),inttov16(-1),inttov16(-1),inttov16(2),inttov16(2),inttov16(2));

		printf("\n64 tests avg. (fixed): %i", (getTimer(0) - time) / 32);
		printf("\nBox Test result: %s", hit ? "hit" : "miss");

		while (GFX_STATUS & (1<<27)); // wait until the geometry engine is not busy

		glGetInt(GL_GET_VERTEX_RAM_COUNT, &vertex_count);
		glGetInt(GL_GET_POLYGON_RAM_COUNT, &polygon_count);

		printf("\n\nRam usage: Culling %s", (keysHeld() & KEY_A) ? "none" : "back faces" );
		printf("\nVertex ram: %i", vertex_count);
		printf("\nPolygon ram: %i", polygon_count);
		printf("\n\nPress A to change culling");
		printf("\n\nPress B to change Ortho vs Persp");
		printf("\nPress Left and Right Up and Down to rotate");
		printf("\nPress L and R to zoom");
		printf("\nTouch screen to rotate cube");

		// flush to the screen
		glFlush();

		//a handy little built in function to wait for a screen refresh
		swiWaitForVBlank();

	}

	return 0;
}
