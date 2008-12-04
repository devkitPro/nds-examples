/*---------------------------------------------------------------------------------

	demo of ansi escape sequences

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	consoleDemoInit();
	
	// ansi escape sequence to clear screen and home cursor
	// /x1b[line;columnH
	iprintf("\x1b[2J");

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("\x1b[10;10HHello World!");

	// ansi escape sequence to move cursor up
	// /x1b[linesA
	iprintf("\x1b[10ALine 0");

	// ansi escape sequence to move cursor left
	// /x1b[columnsD
	iprintf("\x1b[28DColumn 0");

	// ansi escape sequence to move cursor down
	// /x1b[linesB
	iprintf("\x1b[19BLine 19");

	// ansi escape sequence to move cursor right
	// /x1b[columnsC
	iprintf("\x1b[5CColumn 20");

	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}
