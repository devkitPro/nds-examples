/*---------------------------------------------------------------------------------

	Simple console print demo doing window
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touch;

	PrintConsole left = *consoleDemoInit();  
	PrintConsole right = *consoleDemoInit();

	consoleSetWindow(&left, 2,2,10,14);
	consoleSetWindow(&right,16,2,10,14);
	
	while(1) {

		int keys;
		
		scanKeys();
		
		keys = keysHeld();
		
		if(keys & KEY_TOUCH)
		{
			touchRead(&touch);
			
			if(touch.px < 128)
				consoleSelect(&left);
			else
				consoleSelect(&right);
				
			iprintf("T: %i\n", touch.px);
		}

		swiWaitForVBlank();
	}

	return 0;
}
