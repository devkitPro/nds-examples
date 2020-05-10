/******************************************************************************
*******************************************************************************
	
	Easy GL2D 
	Primitives example
	A simple primitives for Easy GL2D DS
	
	Relminator (Richard Eric M. Lope BSN RN)
	Http://Rel.Phatcode.Net
	
	
*******************************************************************************
******************************************************************************/ 


#include <nds.h>
#include <stdio.h>
#include <gl2d.h>




// some useful defines

#define HALF_WIDTH (SCREEN_WIDTH/2)
#define HALF_HEIGHT (SCREEN_HEIGHT/2)
#define BRAD_PI (1 << 14)


// Declarations
void simple(int frame);
void lines(int frame);
void pixels( int frame);

/******************************************************************************

    MAIN 

******************************************************************************/



// Demo titles
const char *demo_titles[] =
{ 
	"Lovely Wobbly  ",
	"OldSkool       ",
	"This Suxxors!  "
};



int main( int argc, char *argv[] )
{

	
	
	// Set it to my favorite mode
	videoSetMode( MODE_5_3D );
	
	
	consoleDemoInit();
	
	
	// Initialize GL in 3d mode
	glScreen2D();
	
	

	iprintf("\x1b[1;1HEasy GL2D Primitives Example");
	
	iprintf("\x1b[3;1HRelminator");
	iprintf("\x1b[4;1HHttp://Rel.Phatcode.Net");
	 
	iprintf("\x1b[6;1HPress Arrow keys to Change FX");
	
	// our ever present frame counter	
	int frame = 0;
	
	int demonum = 0;	// what demo are we currently viewing
	
	int key;		// for key input
	
	
	while( 1 )
	{
	
		// increment frame counter 
		frame++;
		
		// get input
		scanKeys();
		key = keysDown();
		
		
		// process input
		if( (key & KEY_DOWN) || (key & KEY_RIGHT) )
		{
			demonum = (demonum + 1) % 3;
		}
		
		if( (key & KEY_UP) || (key & KEY_LEFT) )
		{
			demonum--;
			if( demonum < 0 ) demonum = 2;
		}
		
		
		// figure out what demo should be viewed
		switch( demonum )
		{
			case 0:
				pixels( frame );
				break;
			case 1:
				lines( frame );
				break;
			case 2:
				simple( frame );
				break;
			default:
				pixels( frame );
		}
		
		glFlush(0);

		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
		
		iprintf("\x1b[10;1HDemo# %i/3", demonum+1);
		iprintf("\x1b[11;1HTitle: %s", demo_titles[demonum] );
	
	}

	return 0;
	
}


// Simple box, triangle, and putpixel demo
void simple(int frame)
{

	// set up GL2D for 2d mode
	glBegin2D();

		// Do some funky color cycling
		int red = abs(sinLerp(frame*220)*31) >> 12 ;
		int green = abs(sinLerp(frame*140)*31) >> 12 ;
		int blue = abs(sinLerp(frame*40)*31) >> 12 ;
		
		// fill the whole screen with a gradient box
		glBoxFilledGradient( 0, 0, 255, 191,
							 RGB15( red,  green,  blue ),
							 RGB15(  blue, 31 - red,  green ),
							 RGB15( green,  blue, 31 - red ),
							 RGB15(  31 - green, red, blue )
						   );
		
		// draw a black box
		glBoxFilled( 200, 10,
					 250, 180,
					 RGB15(0,0,0)
				    );
		
		// draw a border around the black box
		glBox( 200, 10,
			   250, 180,
			   RGB15(0,31,0)
		     );
	
		// draw a triangle
		glTriangleFilled( 20, 100,
						  200, 30,
						  60, 40,
						  RGB15(31,0,31)
						);
	
		// draw a gradient triangle
		glTriangleFilledGradient( 20, 100,
								  200, 30,
								  60, 40,
								  RGB15(blue,red,green),
								  RGB15(green,blue, red),
								  RGB15(red,green,blue)
								);
		

		// translucent mode
		// Poly ID 1
		glPolyFmt(POLY_ALPHA(16) | POLY_CULL_NONE | POLY_ID(1));
		glBoxFilledGradient( 10, 50, 230, 150,
							 RGB15( green,  0,  0 ),
							 RGB15(  0, red,  0 ),
							 RGB15( 31,  0, blue ),
							 RGB15(  0, red, 31 )
						   );

		// translucent mode
		// Poly ID 2
		glPolyFmt(POLY_ALPHA(16) | POLY_CULL_NONE | POLY_ID(2));
		
		glTriangleFilledGradient( 70, 10,
								  20, 130,
								  230, 180,
								  RGB15(red,green,blue),
								  RGB15(blue,red,green),
								  RGB15(green,blue, red)
								);
		                       
		
		// restore to normal(solid) rendering
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(3));
		
		// draw a circle using putpixel
		int i;
		for( i = 0; i < BRAD_PI*2; i += 256)
		{
			int x = (cosLerp(i) * 80 ) >> 12;
			int y = (sinLerp(i) * 70 ) >> 12;
			glPutPixel( HALF_WIDTH + x, HALF_HEIGHT + y, RGB15(red, green, blue) );
		}
			
	glEnd2D();
	

}


// oldskool lines demo
void lines(int frame)
{

	// Do some funky color cycling
	int red = abs(sinLerp(frame*220)*31) >> 12 ;
	int green = abs(sinLerp(frame*140)*31) >> 12 ;
	int blue = abs(sinLerp(frame*40)*31) >> 12 ;
	
	// set up GL2D for 2d mode
	glBegin2D();

		// draw a bunch (4096/32) of colored lines
		// using some funky trig equations
		int i;
		for( i = frame; i < ((1 << 12) + frame); i += 32 )
		{
			int px =  ((sinLerp(frame*130)  * 130)>>12) * cosLerp( (i*100)  );
			int py =  ((sinLerp(frame*280)  * 70)>>12) * sinLerp( (i*200)  );
			int px2 = ((sinLerp(frame*330)  * 100)>>12) * cosLerp( ( (i*300+BRAD_PI))  );
		 	int py2 =  ((sinLerp(frame*140)  * 80)>>12) * sinLerp( ( (i*400+BRAD_PI) )  );
			glLine( HALF_WIDTH + (px >> 12), HALF_HEIGHT + (py >> 12),
					HALF_WIDTH + (px2 >> 12), HALF_HEIGHT + (py2 >> 12),
					RGB15(red,green,blue)
				  );
			glLine( HALF_WIDTH + (py2 >> 12), HALF_HEIGHT + (px >> 12),
					HALF_WIDTH + (py >> 12), HALF_HEIGHT + (px2 >> 12),
					RGB15(green,blue,red)
				  );
		}

	glEnd2D();
	
}


// Some radially displaced pixels
void pixels( int frame)
{

	// Elastic radius
	int radius = 40+ (abs(sinLerp(frame*20)*80) >> 12);
	
	// Do some funky color cycling
	int red = abs(sinLerp(frame*220)*31) >> 12 ;
	int green = abs(sinLerp(frame*140)*31) >> 12 ;
	int blue = abs(sinLerp(frame*40)*31) >> 12 ;
	

	// speed opf animation
	int i = ( frame* 140 ) & 32767;
	
	// duh!
	int angle;
	
	// set up GL2D for 2d mode
	glBegin2D();

		// Draw a full revolution of some radially dispalced pixels
		for( angle = 0; angle < BRAD_PI*2; angle += 64)
		{
		
			int a2 = angle + i;
			int x = cosLerp(angle*2) * radius;
			int y = sinLerp(x/32 + a2) * radius;
			x = cosLerp(y/64 + angle) * (radius+20);
			y = sinLerp(x/64 + a2) * radius;
			int x2 = -y;
			int y2 = x;
			
			glPutPixel( HALF_WIDTH + (x >> 12), 
						HALF_HEIGHT + (y >> 12), 
						RGB15(red,green,blue)
					  );
			glPutPixel( HALF_WIDTH + (x2 >> 12), 
						HALF_HEIGHT + (y2 >> 12), 
						RGB15(green,blue,red)
					  );
			
		}
		
	glEnd2D();
	
	

}