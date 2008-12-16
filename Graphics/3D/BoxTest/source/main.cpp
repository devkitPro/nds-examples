/*---------------------------------------------------------------------------------
	
	Box test to demonstrate 3D bounding box es.  also shows the effect of culling and
	clipping on vertex usage
*--------------------------------------------------------------------------------*/


#include <nds.h>

#include <stdio.h>



//some code for profiling
//---------------------------------------------------------------------------------
u16 startTimer(int timer) {
//---------------------------------------------------------------------------------

	TIMER_CR(timer) = 0;
	TIMER_DATA(0) = 0;
	TIMER_CR(timer) = TIMER_DIV_1 | TIMER_ENABLE;
	return TIMER_DATA(0);
}

#define getTimer(timer) (TIMER_DATA(timer))

//---------------------------------------------------------------------------------
//draws a box...same signature as boxTest
//---------------------------------------------------------------------------------
void DrawBox(float x, float y, float z, float width, float height, float depth) {
//---------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------
int main() {	
//---------------------------------------------------------------------------------

	touchPosition touchXY;

	//put 3D on top
	lcdMainOnTop();

	//setup the sub screen for basic printing
	consoleDemoInit();

	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);

	// initialize gl
	glInit();
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	// Set our view port to be the same size as the screen
	glViewport(0,0,255,191);
	
	//camera
	float rotX = 0, rotY = 0;
	float translate = -5;

	//some profiling code
	u16 time;

	//keep track of vertex ram usage
	int polygon_count;
	int vertex_count;

	//object 
	int rx = 50, ry = 15;
	int oldx = 0, oldy = 0;


	printf("\x1b[10;0HPress A to change culling");
	printf("\n\nPress B to change Ortho vs Persp");
	printf("\nLeft/Right/Up/Down to rotate");
	printf("\nPress L and R to zoom");
	printf("\nTouch screen to rotate cube");

	//main loop
	while (1) {

		//process input
		scanKeys();

		touchRead(&touchXY);

		
		int held = keysHeld();
		int pressed = keysDown();
		
		if( held & KEY_LEFT) rotY++;
		if( held & KEY_RIGHT) rotY--;
		if( held & KEY_UP) rotX ++;
		if( held & KEY_DOWN) rotX --;
		if( held & KEY_L) translate += .1;
		if( held & KEY_R) translate -= .1;

		//reset x and y when user touches screen
		if( pressed & KEY_TOUCH)  {
			oldx = touchXY.px;
			oldy = touchXY.py;
		}

		//if user drags then grab the delta
		if( held & KEY_TOUCH) {
			rx += touchXY.px - oldx; 
			ry += touchXY.py - oldy;
			oldx = touchXY.px;
			oldy = touchXY.py;
		}

		
		//change ortho vs perspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(keysHeld() & KEY_B)
			glOrtho(-4,4,-3,3,0.1,10);	
		else {
			gluPerspective(70, 256.0 / 192.0, 0.1, 10);
		}
		//change cull mode
		if( held & KEY_A)
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

		swiWaitForVBlank();
		printf("\x1b[0;0HBox test cycle count");

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

		printf("\n\nRam usage: Culling %s", ( held & KEY_A) ? "none" : "back faces" );
		printf("\nVertex ram: %i", vertex_count);
		printf("\nPolygon ram: %i", polygon_count);

		// flush to the screen
		glFlush(0);

	}

	return 0;
}
