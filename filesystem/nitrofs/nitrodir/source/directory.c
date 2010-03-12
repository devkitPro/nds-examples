#include <nds.h>
#include <filesystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


//---------------------------------------------------------------------------------
void dirlist(const char* path) {
//---------------------------------------------------------------------------------

	DIR* pdir = opendir(path);
	char *dnbuf;

	if (pdir != NULL) {

		while(true) {
			struct dirent* pent = readdir(pdir);
			if(pent == NULL) break;
			
			if(strcmp(".", pent->d_name) != 0 && strcmp("..", pent->d_name) != 0) {
				dnbuf = (char *)malloc(strlen(pent->d_name)+strlen(path)+2);
				sprintf(dnbuf, "%s/%s", (strcmp("/",path) == 0)?"":path, pent->d_name);
				
				struct stat *statbuf = (struct stat*)malloc(sizeof(statbuf));
				stat(dnbuf, statbuf);

				if(S_ISDIR(statbuf->st_mode)) {
					printf("%s <DIR>\n", dnbuf);
					dirlist(dnbuf);
				} else {
					printf("%s (%d)\n", dnbuf, (int)statbuf->st_size);
				}
				free(dnbuf);
				free(statbuf);
			}
		}
		
		closedir(pdir);
	} else {
		printf("opendir() failure.\n");
	}
}


//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------
	
	// Initialise the console, required for printf
	consoleDemoInit();
	
	if (nitroFSInit()) {

		dirlist("/");

	} else {
		iprintf("nitroFSInit failure: terminating\n");
	}

	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}
