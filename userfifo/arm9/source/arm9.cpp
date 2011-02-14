#include <nds.h>
#include <stdio.h>

int main(void) {
	consoleDemoInit();

	while(1) {

		//ask the custom arm7 program for the temperature
		fifoSendValue32(FIFO_USER_01,0);

		u32 values[3];
		int ctr=0;
		while(ctr<3)
		{
			if(fifoCheckValue32(FIFO_USER_01))
			{
				values[ctr] = fifoGetValue32(FIFO_USER_01);
				ctr++;
			}
		}

		iprintf("%d %d %d\n",values[0],values[1],values[2]);

		swiWaitForVBlank();
		scanKeys();
		if (keysDown() & KEY_START) break;
	}

	return 0;
}
