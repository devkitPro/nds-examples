/*---------------------------------------------------------------------------------

This demo uses the cross pannel resistance to calculate the area of touch contact
A higher value for "area" corresponds to a lower area of contact.  This can be used
to detect the difference between a pen or a finger press and to a certain extent the
relative pressure of the touch.

-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	touchPosition touch;

	consoleDemoInit(); 

	int area = 0;

	while(1) 
	{

		scanKeys();
		int held = keysHeld();

		if(held & KEY_START) break;

		if(held & KEY_TOUCH) {

			touchRead(&touch);

			if (touch.z1 != 0 ) {
				area = touch.px * touch.z2 / touch.z1 - touch.px;
			}

			iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);

			iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

			iprintf("Touch Area (pressure) %04i\n", area);

		}

		swiWaitForVBlank();
	}

	return 0;
}
