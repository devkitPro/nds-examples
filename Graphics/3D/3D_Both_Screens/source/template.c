//-------------------------------------------------------
//
//	3D on both screens
//      Jason Rogers  (aka Dovoto)
//
//-------------------------------------------------------
 
#include <nds.h>
 
//-------------------------------------------------------
void renderCube(int angle){
//-------------------------------------------------------
 
	glPushMatrix();
	glTranslatef(0, 0, -4);
	glRotatef32i(degreesToAngle(angle), inttof32(1), inttof32(1), inttof32(1)); 
	
	glBegin(GL_QUADS);
		glColor3b(255,0,0);		glVertex3f(-1.0f,  1.0f,  1.0f);					
		glColor3b(0,255,0);		glVertex3f( 1.0f,  1.0f,  1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f, -1.0f,  1.0f);					
		glColor3b(255,255,0);	glVertex3f(-1.0f, -1.0f,  1.0f);					

		glColor3b(255,0,0);		glVertex3f(-1.0f,  1.0f, -1.0f);					
		glColor3b(0,255,0);		glVertex3f( 1.0f,  1.0f, -1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f, -1.0f, -1.0f);					
		glColor3b(255,255,0);	glVertex3f(-1.0f, -1.0f, -1.0f);					

		glColor3b(255,0,0);		glVertex3f(-1.0f,  1.0f,  1.0f);					
		glColor3b(0,255,0);		glVertex3f( 1.0f,  1.0f,  1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f,  1.0f, -1.0f);					
		glColor3b(255,255,0);	glVertex3f(-1.0f,  1.0f, -1.0f);					

		glColor3b(255,0,0);		glVertex3f(-1.0f, -1.0f,  1.0f);					
		glColor3b(0,255,0);		glVertex3f( 1.0f, -1.0f,  1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f, -1.0f, -1.0f);					
		glColor3b(255,255,0);	glVertex3f(-1.0f, -1.0f, -1.0f);					

		glColor3b(255,0,0);		glVertex3f( 1.0f, 1.0f,  -1.0f);					
		glColor3b(0,255,0);		glVertex3f( 1.0f, 1.0f,   1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f,-1.0f,   1.0f);					
		glColor3b(255,255,0);	glVertex3f( 1.0f,-1.0f,  -1.0f);					

		glColor3b(255,0,0);		glVertex3f(-1.0f, 1.0f,  -1.0f);					
		glColor3b(0,255,0);		glVertex3f(-1.0f, 1.0f,   1.0f);					
		glColor3b(0,0,255);		glVertex3f(-1.0f,-1.0f,   1.0f);					
		glColor3b(255,255,0);	glVertex3f(-1.0f,-1.0f,  -1.0f);					

	glEnd();
 
	glPopMatrix(1);
}
 
 
//-------------------------------------------------------
void renderPyramid(int angle){
//-------------------------------------------------------
 
	glPushMatrix();
	glTranslatef(0, 0, -4);
	glRotatef32i(degreesToAngle(angle), inttof32(1),inttof32(1),inttof32(1)); 
	
	glBegin(GL_QUADS);
		glColor3b(255,0,0);		glVertex3f(-1.0f, -1.0f,  1.0f);					
		glColor3b(0,255,0);		glVertex3f( 1.0f, -1.0f,  1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f, -1.0f, -1.0f);					
		glColor3b(255,255,0);	glVertex3f(-1.0f, -1.0f, -1.0f);					
	glEnd();

	glBegin(GL_TRIANGLES);
		glColor3b(255,0,0);		glVertex3f( 0.0f,  1.0f,  0.0f);					
		glColor3b(0,255,0);		glVertex3f(-1.0f, -1.0f,  1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f, -1.0f,  1.0f);					

		glColor3b(255,0,0);		glVertex3f( 0.0f,  1.0f,  0.0f);					
		glColor3b(0,255,0);		glVertex3f(-1.0f, -1.0f, -1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f, -1.0f, -1.0f);					

		glColor3b(255,0,0);		glVertex3f( 0.0f,  1.0f,  0.0f);					
		glColor3b(0,255,0);		glVertex3f(-1.0f, -1.0f,  1.0f);					
		glColor3b(0,0,255);		glVertex3f(-1.0f, -1.0f, -1.0f);					

		glColor3b(255,0,0);		glVertex3f( 0.0f,  1.0f,  0.0f);					
		glColor3b(0,255,0);		glVertex3f( 1.0f, -1.0f,  1.0f);					
		glColor3b(0,0,255);		glVertex3f( 1.0f, -1.0f, -1.0f);					
	glEnd();
 
	glPopMatrix(1);
}
 
//-------------------------------------------------------
void renderScene(bool top){
//-------------------------------------------------------
	static short angle = 0;
 
	if(top)
		renderCube(angle);
	else
		renderPyramid(angle);

	angle++;
}
 
 
//-------------------------------------------------------
// set up a 2D layer construced of bitmap sprites
// this holds the image when rendering to the top screen
//-------------------------------------------------------
void initSubSprites(void){
//-------------------------------------------------------
 
	oamInit(&oamSub, SpriteMapping_Bmp_2D_256, false);
 
	int x = 0;
	int y = 0;
 
	int id = 0;

	//set up a 4x3 grid of 64x64 sprites to cover the screen
	for(y = 0; y < 3; y++)
	for(x = 0; x < 4; x++)
	{
	/*	u16 *offset = &SPRITE_GFX_SUB[(x * 64) + (y * 64 * 256)];
 
		oamSet(&oamSub, x + y * 4, x * 64, y * 64, 0, 15, SpriteSize_64x64, 
			SpriteColorFormat_Bmp, offset, -1, false,false,false,false,false);
	*/
		oamSub.oamMemory[id].attribute[0] = ATTR0_BMP | ATTR0_SQUARE | (64 * y);
		oamSub.oamMemory[id].attribute[1] = ATTR1_SIZE_64 | (64 * x);
		oamSub.oamMemory[id].attribute[2] = ATTR2_ALPHA(1) | (8 * 32 * y) | (8 * x);
		id++;
	}
 
	swiWaitForVBlank();
 
	oamUpdate(&oamSub);
}
//-------------------------------------------------------
int main() {		
//-------------------------------------------------------
 
	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_5_2D);
 
	glInit();
 
	// sub sprites hold the bottom image when 3D directed to top
	initSubSprites();
 
	// sub background holds the top image when 3D directed to bottom
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
 
//-------------------------------------------------------
//	 Setup gl
//-------------------------------------------------------
	glEnable(GL_ANTIALIAS);
 
	glClearColor(0,0,0,31); 
	glClearPolyID(63);
	glClearDepth(0x7FFF);
 
	glViewport(0,0,255,191);
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
 
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
 
//-------------------------------------------------------
//	 main loop
//-------------------------------------------------------
	bool top = true;
 
	while (true) 
	{
		// wait for capture unit to be ready
		while(REG_DISPCAPCNT & DCAP_ENABLE);
 
		//-------------------------------------------------------
		//	 Switch render targets
		//-------------------------------------------------------
		top = !top;
 
		if(top)
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
 
		//-------------------------------------------------------
		//	 Render the scene
		//-------------------------------------------------------
		glMatrixMode(GL_MODELVIEW);
 
		renderScene(top);
 
		glFlush(0);
	}
 
	return 0;
}