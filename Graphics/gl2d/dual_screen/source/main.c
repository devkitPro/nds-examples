/******************************************************************************
*******************************************************************************
	
	Easy GL2D 
	Dual Sceen example
	Shows you a simple way to render on both screens in Easy GL2D DS
	* Some code are "borrowed" from the example files
	* Only uses primitives but should work with sprites
	
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

// Example file function to set up
// dual screen 
void initSubSprites(void);


/******************************************************************************

    MAIN 

******************************************************************************/

int main( int argc, char *argv[] )
{

	
	
	// Set it to my favorite mode
	videoSetMode(MODE_5_3D);
	videoSetModeSub(MODE_5_2D);
 
	// init oam to capture 3D scene
	initSubSprites();
 
	// sub background holds the top image when 3D directed to bottom
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
 
	
	// Initialize GL in 3d mode
	glScreen2D();
	
	

	
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
	
		// wait for capture unit to be ready
		while(REG_DISPCAPCNT & DCAP_ENABLE);

	
		// Alternate rendering every other frame
		// Limits your FPS to 30
		if((frame & 1) == 0)
		{
			lcdMainOnBottom();
			vramSetBankC(VRAM_C_LCD);
			vramSetBankD(VRAM_D_SUB_SPRITE);
			REG_DISPCAPCNT = DCAP_BANK(2) | DCAP_ENABLE | DCAP_SIZE(3);
		}
		else
		{
			lcdMainOnTop();
			vramSetBankD(VRAM_D_LCD);
			vramSetBankC(VRAM_C_SUB_BG);
			REG_DISPCAPCNT = DCAP_BANK(3) | DCAP_ENABLE | DCAP_SIZE(3);
		}
 
		// figure out what demo should be viewed
		switch( demonum )
		{
			case 0:			// Alternately draw every other frame
				if ((frame & 1) == 0)
					pixels( frame );
				else
					lines( frame );
				break;
			case 1:
				if ((frame & 1) == 0)
					lines( frame );
				else
					pixels( frame );
				break;
				break;
			case 2:
				if ((frame & 1) == 0)
					simple( frame );
				else
					lines( frame );
				break;
			default:
				if ((frame & 1) == 0)
					pixels( frame );
				else
					lines( frame );
		}
		
		glFlush(0);
		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
		
		
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


//-------------------------------------------------------
// set up a 2D layer construced of bitmap sprites
// this holds the image when rendering to the top screen
//-------------------------------------------------------
void initSubSprites(void)
{
 
	oamInit(&oamSub, SpriteMapping_Bmp_2D_256, false);
 
	int x = 0;
	int y = 0;
 
	int id = 0;

	//set up a 4x3 grid of 64x64 sprites to cover the screen
	for(y = 0; y < 3; y++)
	for(x = 0; x < 4; x++)
	{
		oamSub.oamMemory[id].attribute[0] = ATTR0_BMP | ATTR0_SQUARE | (64 * y);
		oamSub.oamMemory[id].attribute[1] = ATTR1_SIZE_64 | (64 * x);
		oamSub.oamMemory[id].attribute[2] = ATTR2_ALPHA(1) | (8 * 32 * y) | (8 * x);
		id++;
	}
 
	swiWaitForVBlank();
 
	oamUpdate(&oamSub);
}


