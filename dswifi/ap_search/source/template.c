/*---------------------------------------------------------------------------------

Simple wifi demo to locate and connect to an ap
-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <dswifi9.h>

#include <sys/types.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <netdb.h>

//---------------------------------------------------------------------------------
Wifi_AccessPoint* findAP(void){
//---------------------------------------------------------------------------------

	int selected = 0;  
	int i;
	int count = 0, displaytop = 0;

	static Wifi_AccessPoint ap;

	Wifi_ScanMode(); //this allows us to search for APs

	int pressed = 0;
	do {
		scanKeys();

		//find out how many APs there are in the area
		count = Wifi_GetNumAP();


		consoleClear();

		iprintf("%d APs detected\n\n", count);

		int displayend = displaytop + 10;
		if (displayend > count) displayend = count;

		//display the APs to the user
		for(i = displaytop; i < displayend; i++) {
			Wifi_AccessPoint ap;

			Wifi_GetAPData(i, &ap);

			// display the name of the AP
			iprintf("%s %.29s\n  Wep:%s Sig:%i\n", 
				i == selected ? "*" : " ", 
				ap.ssid, 
				ap.flags & WFLAG_APDATA_WEP ? "Yes " : "No ",
				ap.rssi * 100 / 0xD0);

		}

		pressed = keysDown();
		//move the selection asterick
		if(pressed & KEY_UP) {
			selected--;
			if(selected < 0) {
				selected = 0;
			}
			if(selected<displaytop) displaytop = selected;
		}

		if(pressed & KEY_DOWN) {
			selected++;
			if(selected >= count) {
				selected = count - 1;
			}
			displaytop = selected - 9;
			if (displaytop<0) displaytop = 0;
		}
		swiWaitForVBlank();
	} while(!(pressed & KEY_A));

	//user has made a choice so grab the ap and return it
	Wifi_GetAPData(selected, &ap);

	return &ap;
}

//---------------------------------------------------------------------------------
void keyPressed(int c){
//---------------------------------------------------------------------------------
	if(c > 0) iprintf("%c",c);
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	Wifi_InitDefault(false);
	
	consoleDemoInit(); 

	Keyboard* kb = keyboardDemoInit();
	kb->OnKeyPressed = keyPressed;

	while(1) {
		int status = ASSOCSTATUS_DISCONNECTED;

		consoleClear();
		consoleSetWindow(NULL, 0,0,32,24);

		Wifi_AccessPoint* ap = findAP();

		consoleClear();
		consoleSetWindow(NULL, 0,0,32,10);

		iprintf("Connecting to %s\n", ap->ssid);

		//this tells the wifi lib to use dhcp for everything
		Wifi_SetIP(0,0,0,0,0);	
		char wepkey[64];
		int wepmode = WEPMODE_NONE;
		if (ap->flags & WFLAG_APDATA_WEP) {
			iprintf("Enter Wep Key\n");
			while (wepmode == WEPMODE_NONE) {
				scanf("%s",wepkey);
				if (strlen(wepkey)==13) {
					wepmode = WEPMODE_128BIT;
				} else if (strlen(wepkey) == 5) {
					wepmode = WEPMODE_40BIT;
				} else {
					iprintf("Invalid key!\n");
				}
			}
			Wifi_ConnectAP(ap, wepmode, 0, (u8*)wepkey);
		} else {
			Wifi_ConnectAP(ap, WEPMODE_NONE, 0, 0);
		}
		consoleClear();
		while(status != ASSOCSTATUS_ASSOCIATED && status != ASSOCSTATUS_CANNOTCONNECT) {

			status = Wifi_AssocStatus();
			int len = strlen(ASSOCSTATUS_STRINGS[status]);
			iprintf("\x1b[0;0H\x1b[K");
			iprintf("\x1b[0;%dH%s", (32-len)/2,ASSOCSTATUS_STRINGS[status]);

			scanKeys();

			if(keysDown() & KEY_B) break;
			
			swiWaitForVBlank();
		}

		char url[256];

		if(status == ASSOCSTATUS_ASSOCIATED) {
			u32 ip = Wifi_GetIP();

			iprintf("\nip: [%i.%i.%i.%i]\n", (ip ) & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF);
			while(1) {

				scanf("%s", url);

				if ( 0 == strcmp(url,"quit")) break;

				struct hostent *host = gethostbyname(url);

				if(host)
					iprintf("IP (%s) : %s\n",  url, inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));
				else
					iprintf("Could not resolve\n");

				swiWaitForVBlank();
			}
		} else {
			iprintf("\nConnection failed!\n");
		}

		int quit = 0;
		iprintf("Press A to try again, B to quit.");
		while(1) {
			swiWaitForVBlank();
			scanKeys();
			int pressed = keysDown();
			if(pressed&KEY_B) quit = 1;
			if(pressed&(KEY_A|KEY_B)) break;
		}
		if(quit) break;
	}
	return 0;
}
