/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();  //setup the sub screen for printing

	keyboardDemoInit();

	keyboardShow();

	while(pmMainLoop()) {

		int key = keyboardUpdate();

		if(key > 0)
			iprintf("%c", key);

		swiWaitForVBlank();
		scanKeys();

		int pressed = keysDown();

		if(pressed & KEY_START) break;
	}

	return 0;
}
