/*---------------------------------------------------------------------------------

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/

#include <nds.h>
#include <stdio.h>
#include <nds/arm9/piano.h>
#include <nds/arm9/paddle.h>
#include <nds/arm9/guitarGrip.h>

typedef struct {
	union {
		struct {
			u16 c:1;
			u16 c_sharp:1; //rules!
			u16 d:1;
			u16 d_sharp:1;
			u16 e:1;
			u16 f:1;
			u16 f_sharp:1;
			u16 g:1;
			u16 g_sharp:1;
			u16 a:1;
			u16 a_sharp:1;
			u16 _unknown1:1;
			u16 _unknown2:1;
			u16 b:1;
			u16 high_c:1; //is yummy
			u16 _unknown3:1;
		};
		u16 VAL;
	};
} PianoKeys;

int main(void) {
	consoleDemoInit();

	while(1) {
		swiWaitForVBlank();

		consoleClear();
		iprintf("      Hello DS dev'rs\n");
		iprintf("     \x1b[32mwww.devkitpro.org\n");
		iprintf("   \x1b[32;1mwww.drunkencoders.com\x1b[39m");


		iprintf("\x1b[4;0Hgba header 96h: %02X\n",GBA_HEADER.is96h);
		iprintf("0x08000000: %04X\n",*(vu16*)0x08000000);

		if(guitarGripIsInserted())
		{
			guitarGripScanKeys();
			u8 keys = guitarGripKeysHeld();
			iprintf("\x1b[7;0HguitarGrip: %02X\n",keys);
			iprintf("  [%s]  [%s]  [%s]  [%s]",(keys&GUITARGRIP_BLUE)?"BLU":"   ",(keys&GUITARGRIP_YELLOW)?"YEL":"   ",(keys&GUITARGRIP_RED)?"RED":"   ",(keys&GUITARGRIP_GREEN)?"GRN":"   ");
		}

		if(pianoIsInserted())
		{
			pianoScanKeys();
			PianoKeys keys;
			keys.VAL = pianoKeysHeld();
			iprintf("\x1b[7;0Hpiano: %04X\n",pianoKeysHeld());
			iprintf("  %s  %s     %s  %s  %s   \n",keys.c_sharp?"C#":"  ",keys.d_sharp?"D#":"  ",keys.f_sharp?"F#":"  ",keys.g_sharp?"G#":"  ",keys.a_sharp?"A#":"  ");
			iprintf("%s  %s  %s %s  %s  %s  %s  %s\n",keys.c?"C ":"  ",keys.d?"D ":"  ",keys.e?"E ":"  ",keys.f?"F ":"  ",keys.g?"G ":"  ",keys.a?"A ":"  ",keys.b?"B ":"  ",keys.high_c?"C ":"  ");
		}

		if(paddleIsInserted())
		{
			iprintf("\x1b[7;0Hpaddle: %04X\n",paddleRead());
		}


	}

	return 0;
}
