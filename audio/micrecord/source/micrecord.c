#include <nds.h>
#include <stdio.h>

//the record sample rate
#define sample_rate  8000

//buffer to hold sound data for playback
u16* sound_buffer = 0;

//buffer which is written to by the arm7
u16* mic_buffer = 0;

//the length of the current data
u32 data_length = 0;

//enough hold 5 seconds of 16bit audio
u32 sound_buffer_size = sample_rate * 2 * 5;

//the mic buffer sent to the arm7 is a double buffer
//every time it is half full the arm7 signals us so we can read the
//data.  I want the buffer to swap about once per frame so i chose a
//buffer size large enough to hold two frames of 16bit mic data
u32 mic_buffer_size = sample_rate * 2 / 30;


//mic stream handler
void micHandler(void* data, int length)
{
	if(!sound_buffer || data_length > sound_buffer_size) return;
	
	
	DC_InvalidateRange(data, length);

	dmaCopy(data, ((u8*)sound_buffer) + data_length, length);
	
	data_length += length;

	iprintf(".");
	
}

void record(void)
{
	data_length = 0;
	soundMicRecord(mic_buffer, mic_buffer_size, MicFormat_12Bit, sample_rate, micHandler);
}

void play(void)
{
	soundMicOff();
	soundEnable();
	iprintf("data length: %i\n", data_length);
	soundPlaySample(sound_buffer, SoundFormat_16Bit, data_length, sample_rate, 127, 64, false, 0);
}

int main(void) 
{
	int key;
	bool recording = false;

	sound_buffer = (u16*)malloc(sound_buffer_size);
	mic_buffer = (u16*)malloc(mic_buffer_size);

	consoleDemoInit();

	iprintf("Press A to record / play\n");

	while(1) 
	{

		scanKeys();
		key = keysDown();

		if(key & KEY_A)
		{
			recording ? play() : record();
			recording = !recording;
			iprintf("%s\n", recording ? "recording" : "playing");
		}

		swiWaitForVBlank();

	}
}
