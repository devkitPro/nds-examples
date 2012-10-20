#include <nds.h>
#include <maxmod9.h>
#include <filesystem.h>
#include <stdio.h>

#include "soundbank.h"

volatile int frame = 0;

//---------------------------------------------------------------------------------
void Vblank() {
//---------------------------------------------------------------------------------
	frame++;
}
	
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touchXY;

	irqSet(IRQ_VBLANK, Vblank);

	consoleDemoInit();

	nitroFSInit(NULL);

	//define a sound effect's noteon parameters
	mm_sound_effect badong = {
		{ SFX_BADONG } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		255,	// panning
	};

	//initialize maxmod; provide path to soundbank.bin. nitroFS will be used to open the file
	mmInitDefault("soundbank.bin");

	//load and play a sound effect
	mmLoadEffect( SFX_BADONG );
	mmEffectEx(&badong);

	//load and play a mod
	mmLoad( MOD_MOD );
	mmStart( MOD_MOD, MM_PLAY_LOOP );

	//load a script and print it to screen
	FILE* inf = fopen("script.txt","rb");
	static char buf[128];
	while(fgets(buf,128,inf))
	{
		iprintf("%s",buf);
	}
	fclose(inf);

	while(1) {
	
		swiWaitForVBlank();
		touchRead(&touchXY);

		// print at using ansi escape sequence \x1b[line;columnH 
		iprintf("\x1b[10;0HFrame = %d",frame);
		iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);		
	}

	return 0;
}
