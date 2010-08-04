#include <nds.h>

void waitfor(int keys)
{
	scanKeys();
	while((keysDown() & keys) == 0)
	{
		swiWaitForVBlank();
		scanKeys();
	}
}


int channel = 0;
bool play = true;

//this function will be called by the timer.
void timerCallBack()
{
	if(play)
		soundPause(channel);
	else
		soundResume(channel);

	play = !play;
}

int main()
{
	soundEnable();
	channel = soundPlayPSG(DutyCycle_50, 10000, 127, 64);

	//calls the timerCallBack function 5 times per second.
	timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(5), timerCallBack);

	waitfor(KEY_A);

	return 0;
}
