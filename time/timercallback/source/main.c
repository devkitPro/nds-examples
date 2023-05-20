#include <stdio.h>
#include <calico.h>
#include <nds.h>

static bool play = true;
static volatile bool trigger = false;

// Timer callback. This function is executed in IRQ mode - be careful!
void timerCallBack()
{
	play = !play;
	trigger = true;
}

int main()
{
	consoleDemoInit();
	iprintf("Timer callback demo\n");

	soundEnable();
	int channel = soundPlayPSG(DutyCycle_50, 10000, 127, 64);

	//calls the timerCallBack function 5 times per second.
	timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(5), timerCallBack);

	while (pmMainLoop()) {
		swiWaitForVBlank();
		scanKeys();

		if (keysDown() & KEY_START) {
			break;
		}

		if (trigger) {
			trigger = false;
			if (play) {
				soundResume(channel);
			} else {
				soundPause(channel);
			}
		}
	}

	return 0;
}
