#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>

#include "soundbank.h"
#include "soundbank_bin.h"

int main() {

	consoleDemoInit();

	mmInitDefaultMem((mm_addr)soundbank_bin);
	
	// load the module
	mmLoad( MOD_FLATOUTLIES );

	// load sound effects
	mmLoadEffect( SFX_AMBULANCE );
	mmLoadEffect( SFX_BOOM );

	// Start playing module
	mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );

	mm_sound_effect ambulance = {
		{ SFX_AMBULANCE } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	mm_sound_effect boom = {
		{ SFX_BOOM } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		255,	// panning
	};

	// ansi escape sequence to clear screen and home cursor
	// /x1b[line;columnH
	iprintf("\x1b[2J");

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("\x1b[0;8HMaxMod Audio demo");
	iprintf("\x1b[3;0HHold A for ambulance sound");
	iprintf("\x1b[4;0HPress B for boom sound");
	
	// sound effect handle (for cancelling it later)
	mm_sfxhand amb = 0;

	do {

		int keys_pressed, keys_released;
		
		swiWaitForVBlank();
		scanKeys();

		keys_pressed = keysDown();
		keys_released = keysUp();

		// Play looping ambulance sound effect out of left speaker if A button is pressed
		if ( keys_pressed & KEY_A ) {
			amb = mmEffectEx(&ambulance);
		}

		// stop ambulance sound when A button is released
		if ( keys_released & KEY_A ) {
			mmEffectCancel(amb);
		}

		// Play explosion sound effect out of right speaker if B button is pressed
		if ( keys_pressed & KEY_B ) {
			mmEffectEx(&boom);
		}
		if( keys_pressed & KEY_START) break;

	} while(1);
	return 0;
}
