/*---------------------------------------------------------------------------------

	Simple console print demo which prints to both screens
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touch;

	PrintConsole topScreen, bottomScreen;

	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	//create two consoles
	consoleNew(&topScreen);
	consoleNew(&bottomScreen);

	//initialize their backgrounds
	topScreen.bgId = bgInit(3,BgType_Text4bpp, BgSize_T_256x256, 31, 0);
	bottomScreen.bgId = bgInitSub(3,BgType_Text4bpp, BgSize_T_256x256, 31, 0);

	//everthing else we leave the same
	consoleInit(&topScreen);
	consoleInit(&bottomScreen);

	consoleSelect(&topScreen);
	iprintf("\n\n\tHello DS dev'rs\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\twww.devkitpro.org");

	consoleSelect(&bottomScreen);

	while(1) {

		touchRead(&touch);
		
		iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);
		iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

		swiWaitForVBlank();
	}

	return 0;
}
