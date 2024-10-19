/*---------------------------------------------------------------------------------

	ARM7 code for PXI example
	-- fincs

---------------------------------------------------------------------------------*/
#include <nds.h>

// Management structure and stack space for PXI server thread
static Thread s_myServerThread;
alignas(8) static u8 s_myServerThreadStack[1024];

//---------------------------------------------------------------------------------
static int myServerThreadMain(void* arg) {
//---------------------------------------------------------------------------------
	// Set up PXI mailbox, used to receive PXI command words
	Mailbox mb;
	u32 mb_slots[4];
	mailboxPrepare(&mb, mb_slots, sizeof(mb_slots)/4);
	pxiSetMailbox(PxiChannel_User0, &mb);

	// Main PXI message loop
	for (;;) {
		// Receive a message
		u32 msg = mailboxRecv(&mb);
		u32 retval = 0;

		switch (msg) {
			default: break;

			// Command 0: Read the firmware chip's JEDEC identifier
			case 0: {
				spiLock();
				nvramReadJedec(&retval);
				spiUnlock();

				break;
			}

			// Command 1: Read touch pressure values (only works on DS Phat/DS Lite)
			case 1: {
				unsigned z1 = 0, z2 = 0;

				if (!cdcIsTwlMode()) {
					spiLock();
					z1 = tscReadChannel12(TscChannel_Z1);
					z2 = tscReadChannel12(TscChannel_Z2);
					spiUnlock();
				}

				// Pack the two 12-bit values into the reply value
				retval = z1 | (z2 << 12);
				break;
			}
		}

		// Send a reply back to the ARM9
		pxiReply(PxiChannel_User0, retval);
	}

	return 0;
}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Read settings from NVRAM
	envReadNvramSettings();

	// Set up extended keypad server (X/Y/hinge)
	keypadStartExtServer();

	// Configure and enable VBlank interrupt
	lcdSetIrqMask(DISPSTAT_IE_ALL, DISPSTAT_IE_VBLANK);
	irqEnable(IRQ_VBLANK);

	// Set up RTC
	rtcInit();
	rtcSyncTime();

	// Initialize power management
	pmInit();

	// Set up touch screen driver
	touchInit();
	touchStartServer(80, MAIN_THREAD_PRIO);

	// Set up server thread
	threadPrepare(&s_myServerThread, myServerThreadMain, NULL, &s_myServerThreadStack[sizeof(s_myServerThreadStack)], MAIN_THREAD_PRIO);
	threadStart(&s_myServerThread);

	// Keep the ARM7 mostly idle
	while (pmMainLoop()) {
		threadWaitForVBlank();
	}

	return 0;
}
