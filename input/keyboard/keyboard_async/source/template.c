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

	while(1) {
		
		int key = keyboardUpdate();

		if(key > 0)
			iprintf("%c", key);

		swiWaitForVBlank();
	}

	return 0;
}
