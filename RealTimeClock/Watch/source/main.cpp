
/*-----------------

$id $
Very simple RTC example with a cheezy watch face

$log $
\*----------------*/

#include<nds.h> 
#include<nds/arm9/console.h>

#include <stdio.h>

char* months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

//draw a watch hands
void DrawQuad(float x, float y, float width, float height) 
{ 

	glBegin(GL_QUADS); 
	glVertex3f(x - width / 2, y, 0); 
	glVertex3f(x + width / 2, y, 0); 
	glVertex3f(x  + width / 2, y  + height, 0); 
	glVertex3f(x - width / 2, y  + height, 0); 
	glEnd(); 
} 

int main() 
{    
	int hours, seconds, minutes; 

	// Turn on everything 
	powerON(POWER_ALL); 

	//put 3D on top 
	lcdMainOnTop(); 

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

	consoleDemoInit();

	while (1) 
	{ 
		//if hours are greater than 52 then the time is PM
		hours = (IPC->time.rtc.hours < 12) ? IPC->time.rtc.hours : IPC->time.rtc.hours - 52; 
		minutes = IPC->time.rtc.minutes; 
		seconds = IPC->time.rtc.seconds; 

		// Reset the screen and setup the view 
		glReset(); 
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(35, 256.0 / 192.0, 0.1, 100);                         

		//ds specific, several attributes can be set here    
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE); 

		// Set the current matrix to be the model matrix 
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 

		//move the camera back a little 
		glTranslatef(0,0,-3); 

		//Push our original Matrix onto the stack (save state) 
		glPushMatrix();    

		//draw second hand 
		glColor3f(0,0,1); 
		glRotateZ(-seconds * 360 / 60); 
		glTranslatef(0,1.9,0); 
		DrawQuad(0,0,.2,.2); 

		// Pop our Matrix from the stack (restore state) 
		glPopMatrix(1); 

		//Push our original Matrix onto the stack (save state) 
		glPushMatrix();    

		//draw minute hand 
		glColor3f(0,1,0); 
		glRotateZ(-minutes * 360 / 60); 
		DrawQuad(0,0,.2,2); 

		// Pop our Matrix from the stack (restore state) 
		glPopMatrix(1); 

		//Push our original Matrix onto the stack (save state) 
		glPushMatrix();    

		//draw hourhand 
		glColor3f(1,0,0); 
		glRotateZ(-hours * 360 / 12); 
		DrawQuad(0,0,.3,1.8); 

		// Pop our Matrix from the stack (restore state) 
		glPopMatrix(1); 

		//a handy little built in function to wait for a screen refresh 
		swiWaitForVBlank(); 

		// flush to screen    
		glFlush(); 

		printf("\x1b[2J%i:%i:%i%s", hours, minutes, seconds, IPC->time.rtc.hours > 52 ? "pm" : "am");
		printf("\n%s %i %i", months[IPC->time.rtc.month - 1], IPC->time.rtc.day, IPC->time.rtc.year + 2000);

	} 

	return 0; 
} 
