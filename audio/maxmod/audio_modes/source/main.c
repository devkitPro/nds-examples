/***********************************************
 * this example demonstrates the 3 audio modes
 *
 * functions used:
 *   mmInitDefaultMem(soundbank)
 *     Initialize with default settings
 *
 *   mmLoad( module )
 *     Loads a module to be played
 *
 *   mmStart( module )
 *     Starts playback of a module
 *
 *   mmStop()
 *     Stops module playback
 *
 *   mmSelectMode( mode )
 *     Selects the audio mode
 *     modes:
 *       0: MM_MODE_A, hardware audio mode
 *       1: MM_MODE_B, interpolated audio mode
 *       2: MM_MODE_C, extended audio mode
 *********************************************************/
 
#include <nds.h>
#include <maxmod9.h>

#include <stdio.h>

#include "soundbank_bin.h"
#include "soundbank.h"

//---------------------------------------------
const char *SHOW_TEXT = "\n"

"   Maxmod Audio Modes Example\n\n"

" Song: \n"
" Mode: \n\n"

" Left/Right: Select Song\n"
" Up/Down: Change Audio Mode\n"
" A: Start Playback\n"
" B: Stop Playback\n\n"

" Tip: Play subtonal with the\n"
" extended mode or else it won't\n"
" sound right.\n\n"

" Another Tip: The interpolated\n"
" mode doesn't work in current\n"
" emulators.";
//---------------------------------------------

void print_song( int song );
void print_mode( int mode );

// song order
const short song_order[] = { MOD_KEYG_SUBTONAL, MOD_REZ_MONDAY, MOD_PURPLE_MOTION_INSPIRATION };

// rez-monday.mod is a bit loud, so we will lower the volume to 500 (normal volume is 1024)
const short song_volumes[] = { 1024, 500, 1024 };

// strings for the "Song: " display
const char *song_titles[] = { 
	"subtonal (30ch)  ",
	"monday (14ch)    ",
	"inspiration (4ch)"
};

// strings for the "Mode: " display
const char *audiomode_names[] = {
	"A - Hardware (16ch)    ",
	"B - Interpolated (16ch)",
	"C - Extended (30ch)    "
};

//---------------------------------------------------------------------------------
// program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	
	//---------------------------------------------------------
	// setup console
	//---------------------------------------------------------
	consoleDemoInit();
	
	// set a dark blue backdrop
	BG_PALETTE_SUB[0] = RGB15( 0, 0, 10 );
	
	//---------------------------------------------------------
	// init maxmod with default settings
	//---------------------------------------------------------
	mmInitDefaultMem( (mm_addr)soundbank_bin );
	
	//---------------------------------------------------------
	// load songs (must be loaded before using with mmStart)
	//---------------------------------------------------------
	mmLoad( MOD_KEYG_SUBTONAL );
	mmLoad( MOD_REZ_MONDAY );
	mmLoad( MOD_PURPLE_MOTION_INSPIRATION );
	
	int song = 0;
	int mode = 0;
	
	//---------------------------------------------------------
	// display screen info
	//---------------------------------------------------------
	iprintf( SHOW_TEXT );
	print_song( song );
	print_mode( mode );
	
	//---------------------------------------------------------
	// main loop
	//---------------------------------------------------------
	while(1)
	{
		//-----------------------------------------------------
		// get new keypad input
		//-----------------------------------------------------
		scanKeys();
		int keys = keysDown();
		
		//-----------------------------------------------------
		// LEFT: select previous song
		//-----------------------------------------------------
		if( keys & KEY_LEFT )
		{
			song--;
			if( song < 0 ) song = 2;
			print_song( song );
		}
		
		//-----------------------------------------------------
		// RIGHT: select next song
		//-----------------------------------------------------
		if( keys & KEY_RIGHT )
		{
			song++;
			if( song > 2 ) song = 0;
			print_song( song );
		}
		
		//-----------------------------------------------------
		// A: start song
		//-----------------------------------------------------
		if( keys & KEY_A )
		{
			mmSetModuleVolume( song_volumes[song] );
			
			// loop module until stopped with B keypress
			mmStart( song_order[song], MM_PLAY_LOOP );
		}
		
		//-----------------------------------------------------
		// B: stop song
		//-----------------------------------------------------
		if( keys & KEY_B )
		{
			mmStop();
		}
	
		//-----------------------------------------------------
		// UP: next audio mode
		//-----------------------------------------------------
		if( keys & KEY_UP )
		{
			mode++;
			if( mode > 2 ) mode = 0;
			print_mode( mode );
			
			// switch audio mode
			mmSelectMode( mode );
		}
		
		//-----------------------------------------------------
		// DOWN: previous audio mode
		//-----------------------------------------------------
		if( keys & KEY_DOWN )
		{
			mode--;
			if( mode < 0 ) mode = 2;
			print_mode( mode );
			
			// switch audio mode
			mmSelectMode( mode );
		}
		
		//-----------------------------------------------------
		// wait until next frame
		//-----------------------------------------------------
		swiWaitForVBlank();
	}
	
	return 0;
}

//---------------------------------------------------------------------------------
// print song title
//---------------------------------------------------------------------------------
void print_song( int song ) {
//---------------------------------------------------------------------------------
	iprintf("\x1b[3;7H%s", song_titles[song] );
}

//---------------------------------------------------------------------------------
// print audio mode
//---------------------------------------------------------------------------------
void print_mode( int mode ) {
//---------------------------------------------------------------------------------
	iprintf("\x1b[4;7H%s", audiomode_names[mode] );
}
