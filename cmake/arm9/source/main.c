/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();
	iprintf("Hello World!");
	while(1) {
		swiWaitForVBlank();
		scanKeys();
		int pressed = keysDown();
		if(pressed & KEY_START) break;
	}

}
