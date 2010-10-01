#include <nds.h>

#include <stdio.h>
#include <time.h>

//the speed of the timer when using ClockDivider_1024
#define TIMER_SPEED (BUS_CLOCK/1024)

typedef enum
{
	timerState_Stop,
	timerState_Pause,
	timerState_Running
}TimerStates;


int main()
{
	consoleDemoInit();

	uint ticks = 0;
	TimerStates state = timerState_Stop;
	int down = keysDown();

	while(!(down & KEY_START))
	{
		swiWaitForVBlank();
		consoleClear();
		scanKeys();
		down = keysDown();

		if(state == timerState_Running)
		{
			ticks += timerElapsed(0);
		}

		if(down & KEY_A)
		{
			if(state == timerState_Stop)
			{
				timerStart(0, ClockDivider_1024, 0, NULL);
				state = timerState_Running;
			}
			else if(state == timerState_Pause)
			{
				timerUnpause(0);
				state = timerState_Running;
			}
			else if(state == timerState_Running)
			{
				ticks += timerPause(0);
				state = timerState_Pause;
			}
		}
		else if(down & KEY_B)
		{
			timerStop(0);
			ticks = 0;
			state = timerState_Stop;
		}

		iprintf("Press A to start and pause the \ntimer, B to clear the timer \nand start to quit the program.\n\n");
		iprintf("ticks:  %u\n", ticks);
		iprintf("second: %u:%u\n", ticks/TIMER_SPEED, ((ticks%TIMER_SPEED)*1000) /TIMER_SPEED);
	}

	if(state != timerState_Stop)
	{
		timerStop(0);
	}

	return 0;
}
