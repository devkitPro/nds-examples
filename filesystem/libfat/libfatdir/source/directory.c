#include <nds.h>
#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------
	
	// Initialise the console, required for printf
	consoleDemoInit();
	
	if (fatInitDefault()) {

	
		DIR *pdir;
		struct dirent *pent;
		struct stat statbuf;

		pdir=opendir("/");

		if (pdir){

			while ((pent=readdir(pdir))!=NULL) {
	    		stat(pent->d_name,&statbuf);
	    		if(strcmp(".", pent->d_name) == 0 || strcmp("..", pent->d_name) == 0)
	        		continue;
	    		if(S_ISDIR(statbuf.st_mode))
	        		iprintf("%s <dir>\n", pent->d_name);
	    		if(!(S_ISDIR(statbuf.st_mode)))
	        		iprintf("%s %ld\n", pent->d_name, statbuf.st_size);
			}
			closedir(pdir);
		} else {
			iprintf ("opendir() failure; terminating\n");
		}

	} else {
		iprintf("fatInitDefault failure: terminating\n");
	}

	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}
