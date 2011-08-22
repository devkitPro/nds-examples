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

	if (pdir != NULL) {

		while(true) {
			struct dirent* pent = readdir(pdir);
			if(pent == NULL) break;
			
			if(strcmp(".", pent->d_name) != 0 && strcmp("..", pent->d_name) != 0) {
				if(pent->d_type == DT_DIR) {
					printf("%s/%s <DIR>\n", (strcmp("/",path) == 0)?"":path, pent->d_name);
					char *dnbuf = (char *)malloc(strlen(pent->d_name)+strlen(path)+2);
					sprintf(dnbuf, "%s/%s", (strcmp("/",path) == 0)?"":path, pent->d_name);
					dirlist(dnbuf);
					free(dnbuf);
				} else {
					printf("%s/%s\n",(strcmp("/",path) == 0)?"":path, pent->d_name);
				}
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
	
	if (nitroFSInit(NULL)) {

		dirlist("/");
		
		{
			// now, try reading a file to make sure things are working OK.
			FILE* inf = fopen("file1.txt","rb");
			if(inf)
			{
				int len;

				fseek(inf,0,SEEK_END);
				len = ftell(inf);
				fseek(inf,0,SEEK_SET);

				iprintf("\nthe following %d bytes message\nfrom file1.txt is\nbrought to you by fread:\n",len);
				{
					char *entireFile = (char*)malloc(len+1);
					entireFile[len] = 0;
					if(fread(entireFile,1,len,inf) != len)
						iprintf("savage error reading the bytes from the file!\n");
					else
						iprintf("%s\n-done-\n",entireFile);
					free(entireFile);
				}

				fclose(inf);
			}
		}

		iprintf("here is the dirlist once more:\n");
		dirlist("/");

	} else {
		iprintf("nitroFSInit failure: terminating\n");
	}

	while(1) {
		swiWaitForVBlank();
		scanKeys();
		if(keysDown()&KEY_START) break;
	}

	return 0;
}
