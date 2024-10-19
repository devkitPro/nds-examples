/*---------------------------------------------------------------------------------

	ARM9 code for PXI example
	-- fincs

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
static s32 calcTouchPressure(unsigned px, unsigned z1, unsigned z2) {
//---------------------------------------------------------------------------------
	// Given px, z1, z2 (all 20.12 fixed point numbers),
	// pressure = 1/resistance = 1 / (x * (z2/z1 - 1)) = z1 / (x * (z2 - z1))
	// With some mathematical cleverness we can avoid divisions by zero,
	// as well as 0/0 indetermination (such as when z1 == z2)

	s32 num = divf32(z1, px);
	s32 den = z2 - z1;

	if (num == den) {
		return inttof32(1);
	} else {
		return divf32(num, den);
	}
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touch;

	consoleDemoInit();  //setup the sub screen for printing

	pxiWaitRemote(PxiChannel_User0);

	iprintf("\n\n\tHello DS dev'rs\n");
	iprintf("\thttps://devkitpro.org\n\n");

	u32 jedec = pxiSendAndReceive(PxiChannel_User0, 0);
	printf("Firmware JEDEC ID: 0x%06lX\n", jedec);

	while(pmMainLoop()) {

		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;

		if (keysHeld()&KEY_TOUCH) {
			touchRead(&touch);

			u32 reply = pxiSendAndReceive(PxiChannel_User0, 1);
			s32 pressure = calcTouchPressure(touch.rawx, reply & 0xfff, reply >> 12);

			iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);
			iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

			printf("Touch pressure: %.6f\n", f32tofloat(pressure));
		}

	}

	return 0;
}
