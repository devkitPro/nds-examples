
/*-----------------

Very simple RTC example with a cheesy watch face

\*----------------*/

#include <nds.h>

#include <stdio.h>
#include <time.h>

const char* months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char* weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const u16 daysAtStartOfMonthLUT[12] =
{
	0	%7, //januari		31
	31	%7, //februari		28+1(leap year)
	59	%7, //maart			31
	90	%7, //april			30
	120	%7, //mei			31
	151	%7, //juni			30
	181	%7, //juli			31
	212	%7, //augustus		31
	243	%7, //september		30
	273	%7, //oktober		31
	304	%7, //november		30
	334	%7  //december		31
};

#define isLeapYear(year) (((year)%4) == 0)

uint getDayOfWeek(uint day, uint month, uint year)
{
	//http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week

	day += 2*(3-((year/100)%4));
	year %= 100;
	day += year + (year/4);
	day += daysAtStartOfMonthLUT[month] - (isLeapYear(year) && (month <= 1));
	return day % 7;
}



void init3D();
void update3D(int hours, int seconds, int minutes);


int main()
{
	int hours, seconds, minutes, day, month, year;

	consoleDemoInit();

	init3D();

	while(true)
	{
		time_t unixTime = time(NULL);
		struct tm* timeStruct = gmtime((const time_t *)&unixTime);

		hours = timeStruct->tm_hour;
		minutes = timeStruct->tm_min;
		seconds = timeStruct->tm_sec;
		day = timeStruct->tm_mday;
		month = timeStruct->tm_mon;
		year = timeStruct->tm_year +1900;

		printf("\x1b[2J%02i:%02i:%02i", hours, minutes, seconds);
		printf("\n%s %s %i %i", weekDays[getDayOfWeek(day, month, year)], months[month], day, year);


		update3D(hours, seconds, minutes);

		swiWaitForVBlank();
	}

	return 0;
}





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

void init3D()
{
	//put 3D on top
	lcdMainOnTop();

	// Setup the Main screen for 3D
	videoSetMode(MODE_0_3D);

	// Reset the screen and setup the view
	glInit();

	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);

	// Specify the Clear Color and Depth
	glClearColor(0,0,0,31);
	glClearDepth(0x7FFF);

	//ds specific, several attributes can be set here
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

	// Set the current matrix to be the model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
	gluLookAt(	0.0, 0.0, 3.0,		//camera possition
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
}


void update3D(int hours, int seconds, int minutes)
{
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

	// flush to screen
	glFlush(0);
}


