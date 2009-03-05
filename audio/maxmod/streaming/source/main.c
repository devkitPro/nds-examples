/****************************************************************
 * this example demonstrates streaming synthensized audio
 ****************************************************************/
 
#include <nds.h>
#include <stdio.h>
#include <maxmod9.h>

int sine;		// sine position
int lfo;		// LFO position

//-----------------------------------------------------------------------------
enum {
//-----------------------------------------------------------------------------
	// waveform base frequency
	sine_freq = 500,
	
	// LFO frequency
	lfo_freq = 3,
	
	// LFO output shift amount
	lfo_shift = 4,
	
	// blue backdrop
	bg_colour = 13 << 10,
	
	// red cpu usage
	cpu_colour = 31
};

/***********************************************************************************
 * on_stream_request
 *
 * Audio stream data request handler.
 ***********************************************************************************/
mm_word on_stream_request( mm_word length, mm_addr dest, mm_stream_formats format ) {
//----------------------------------------------------------------------------------
	
	s16 *target = dest;
	
	//------------------------------------------------------------
	// synthensize a sine wave with an LFO applied to the pitch
	// the stereo data is interleaved
	//------------------------------------------------------------
	int len = length;
	for( ; len; len-- )
	{
		int sample = sinLerp(sine);
		
		// output sample for left
		*target++ = sample;
		
		// output inverted sample for right
		*target++ = -sample;
		
		sine += sine_freq + (sinLerp(lfo) >> lfo_shift);
		lfo = (lfo + lfo_freq);
	}
	
	return length;
}

/**********************************************************************************
 * main
 *
 * Program Entry Point
 **********************************************************************************/
int main( void ) {
//---------------------------------------------------------------------------------

	//----------------------------------------------------------------
	// print out some stuff
	//----------------------------------------------------------------
	consoleDemoInit();
	iprintf( "\n    Maxmod Streaming Example   \n");

	//----------------------------------------------------------------
	// initialize maxmod without any soundbank (unusual setup)
	//----------------------------------------------------------------
	mm_ds_system sys;
	sys.mod_count 			= 0;
	sys.samp_count			= 0;
	sys.mem_bank			= 0;
	sys.fifo_channel		= FIFO_MAXMOD;
	mmInit( &sys );
	
	//----------------------------------------------------------------
	// open stream
	//----------------------------------------------------------------
	mm_stream mystream;
	mystream.sampling_rate	= 25000;					// sampling rate = 25khz
	mystream.buffer_length	= 1200;						// buffer length = 1200 samples
	mystream.callback		= on_stream_request;		// set callback function
	mystream.format			= MM_STREAM_16BIT_STEREO;	// format = stereo 16-bit
	mystream.timer			= MM_TIMER0;				// use hardware timer 0
	mystream.manual			= true;						// use manual filling
	mmStreamOpen( &mystream );
		
	//----------------------------------------------------------------
	// when using 'automatic' filling, your callback will be triggered
	// every time half of the wave buffer is processed.
	//
	// so: 
	// 25000 (rate)
	// ----- = ~21 Hz for a full pass, and ~42hz for half pass
	// 1200  (length)
	//----------------------------------------------------------------
	// with 'manual' filling, you must call mmStreamUpdate
	// periodically (and often enough to avoid buffer underruns)
	//----------------------------------------------------------------
	
	SetYtrigger( 0 );
	irqEnable( IRQ_VCOUNT );
	
	while( 1 )
	{
		// wait until line 0
		swiIntrWait( 0, IRQ_VCOUNT);
		
		// update stream
		mmStreamUpdate();
		
		// restore backdrop (some lines were drawn with another colour to show cpu usage)
		BG_PALETTE_SUB[0] = bg_colour;
		
		// wait until next frame
		swiWaitForVBlank();
		
		// set backdrop to show cpu usage
		BG_PALETTE_SUB[0] = cpu_colour;
	}
	
	return 0;
}
