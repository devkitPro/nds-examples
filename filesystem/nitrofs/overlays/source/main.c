/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <fat.h>
#include <stdio.h>

const char* test0Func(void);
void test1FuncInItcm(void);
void test2Func(void);

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------

	consoleDemoInit();

	iprintf("DS ROM overlay demo\n");

	if (!fatInitDefault()) {
		iprintf("[WARN] fatInitDefault failed\n");
	}

	if (!ovlInit()) {
		iprintf("[ERROR] ovlInit failed\n");
	} else for (unsigned i = 0; i < 2; i ++) {
		if (!ovlLoadAndActivate(0)) {
			iprintf("[ERROR] test0 load fail\n");
		} else {
			iprintf("Message: %s\n", test0Func());
			ovlDeactivate(0);
		}

		if (!ovlLoadAndActivate(1)) {
			iprintf("[ERROR] test1 load fail\n");
		} else {
			test1FuncInItcm();
			ovlDeactivate(1);
		}

		if (!ovlLoadAndActivate(2)) {
			iprintf("[ERROR] test2 load fail\n");
		} else {
			test2Func();
			test2Func();
			test2Func();
			ovlDeactivate(2);
		}
	}

	while (pmMainLoop()) {
		threadWaitForVBlank();
		scanKeys();

		unsigned pressed = keysDown();
		if (pressed & KEY_START) {
			break;
		}
	}

	return 0;

}
