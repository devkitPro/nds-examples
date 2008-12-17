/*********************************************
 * this example demonstrates hardware reverb *
 *********************************************/

#include <nds.h>
#include <stdio.h>

#include <maxmod9.h>			// maxmod reference
#include "soundbank_bin.h"		// soundbank binary reference
#include "soundbank.h"			// generated soundbank definitions

//---------------------------------------------------------------------------------
const char *SCREEN_TEXT = "\n"

" Maxmod Reverb Example\n\n"

" Press A to toggle reverb. \n\n"
;
//---------------------------------------------

enum {
	rv_delay_left = 500,		// milliseconds
	rv_delay_right = 520,
	rv_rate = 32768,			// Hertz
	rv_format = 16				// 16-bit
};

//---------------------------------------------------------------------------------
void setupReverb( void ) {
//---------------------------------------------------------------------------------

	//---------------------------------------------------------
	// Enable reverb system
	//---------------------------------------------------------
	mmReverbEnable();
	
	//---------------------------------------------------------
	// Calculate amount required and allocate memory
	// for both reverb channels
	//---------------------------------------------------------
	void* rv_buffer_left;
	void* rv_buffer_right;

	int rv_size_left;
	int rv_size_right;
	rv_size_left = mmReverbBufferSize( rv_format, rv_rate, rv_delay_left );
	rv_size_right = mmReverbBufferSize( rv_format, rv_rate, rv_delay_right );
	
	rv_buffer_left  = malloc( rv_size_left * 4 );
	rv_buffer_right = malloc( rv_size_right * 4 );
	
	//---------------------------------------------------------
	// Configure reverb settings
	//---------------------------------------------------------
	mm_reverb_cfg config;
	
	config.flags = MMRF_MEMORY | MMRF_DELAY | MMRF_FEEDBACK |
				   MMRF_PANNING | MMRF_DRYLEFT | MMRF_DRYRIGHT |
				   MMRF_RATE | MMRF_16BITLEFT | MMRF_16BITRIGHT |
				   MMRF_INVERSEPAN | MMRF_BOTH;

	config.memory = rv_buffer_left;		// Set memory target (for left)
	config.delay = rv_size_left;		// Set delay (for left)
	config.feedback = 1024;				// Set feedback to 50% (for both)
	config.panning = 0;					// Set panning to 0% (and inversed (100%) for right channel)
	config.rate = 16777216 / rv_rate;	// Set sampling rate for both channels
	mmReverbConfigure( &config );		// Run configuration...

	//---------------------------------------------------------
	// Configure remaining parameters (right channel memory
	// and delay)
	//---------------------------------------------------------
	config.flags = MMRF_MEMORY | MMRF_DELAY | MMRF_RIGHT;
	
	config.delay = rv_size_right;
	config.memory = rv_buffer_right;
	
	mmReverbConfigure( &config );
	
}

//---------------------------------------------------------------------------------
// program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	//---------------------------------------------------------
	// setup console
	//---------------------------------------------------------
	consoleDemoInit();
	
	// give it a dark blue backdrop
	BG_PALETTE_SUB[0] = RGB15( 0, 0, 13 );

	//---------------------------------------------------------
	// init maxmod with default settings
	//---------------------------------------------------------
	mmInitDefaultMem( (mm_addr)soundbank_bin );
	
	//---------------------------------------------------------
	// setup and configure the reverb system
	//---------------------------------------------------------
	setupReverb();
	
	//---------------------------------------------------------
	// load and play test module
	//---------------------------------------------------------
	mmLoad( MOD_TEMPEST_ZEN_BOWLING );
	mmStart( MOD_TEMPEST_ZEN_BOWLING, MM_PLAY_LOOP );
	
	//---------------------------------------------------------
	// display screen info
	//---------------------------------------------------------
	iprintf( SCREEN_TEXT );
	
	int reverb_is_started = 0;
	
	//---------------------------------------------------------
	// main loop
	//---------------------------------------------------------
	while(1) {
		//-----------------------------------------------------
		// get new keypad input
		//-----------------------------------------------------
		scanKeys();
		int keys = keysDown();

		//-----------------------------------------------------
		// A: toggle reverb
		//-----------------------------------------------------
		if( keys & KEY_A ) {
			if( !reverb_is_started ) {
				// start reverb
				mmReverbStart( MMRC_BOTH );
				
				// lower module volume & tempo
				// this creates a cool atmosphere
				mmSetModuleVolume( 550 );
				mmSetModuleTempo( 800 );
				
				reverb_is_started = 1;
			} else {
				// stop reverb and restore volume and tempo
				mmReverbStop( MMRC_BOTH );
				mmSetModuleVolume( 0x400 );
				mmSetModuleTempo( 0x400 );
				
				reverb_is_started = 0;
			}
		}
		
		//-----------------------------------------------------
		// wait until next frame
		//-----------------------------------------------------
		swiWaitForVBlank();
	}
	
	return 0;
}
