/*---------------------------------------------------------------------------------

	Simple console print demo using two consols with different windows
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

char *border = 
"------------"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"|          |"
"------------";

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touch;

	PrintConsole *left = consoleDemoInit();  
	PrintConsole right = *left;	
	
	consoleSetWindow(left, 15,1,12,16);
	consoleSetWindow(&right, 1,1,12,16);

	consoleSelect(left);
	iprintf(border);
	consoleSelect(&right);
	iprintf(border);

	consoleSetWindow(left, 2,2,10,14);
	consoleSetWindow(&right,16,2,10,14);
	
	while(1) 
	{
		int keys;
		
		scanKeys();
		
		keys = keysHeld();
		
		if(keys & KEY_TOUCH)
		{
			touchRead(&touch);
			
			if(touch.px < 128)
				consoleSelect(left);
			else
				consoleSelect(&right);
				
			iprintf("\nT: %i", touch.px);
		}

		swiWaitForVBlank();
	}

	return 0;
}
