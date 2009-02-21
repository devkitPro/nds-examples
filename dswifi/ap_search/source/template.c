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
	int count = 0;

	static Wifi_AccessPoint ap;

	Wifi_ScanMode(); //this allows us to search for APs

	while(!(keysDown() & KEY_A))
	{
		scanKeys();

		//find out how many APs there are in the area
		count = Wifi_GetNumAP();

		consoleClear();

		iprintf("Number of APs found: %d\n", count);

		//display the APs to the user
		for(i = 0; i < count; i++)
		{
			Wifi_AccessPoint ap;

			Wifi_GetAPData(i, &ap);

			// display the name of the AP
			iprintf("%s %s Wep:%s Sig:%i\n", 
				i == selected ? "*" : " ", 
				ap.ssid, 
				ap.flags & WFLAG_APDATA_WEP ? "Yes " : "No ",
				ap.rssi * 100 / 0xD0);

		}

		//move the selection asterick
		if(keysDown() & KEY_UP)
		{
			selected--;
			if(selected < 0)
			{
				selected = 0;
			}
		}

		if(keysDown()&KEY_DOWN)
		{
			selected++;
			if(selected >= count) 
			{
				selected = count - 1;
			}
		}

		swiWaitForVBlank();
	}

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

	while(1)
	{
		int status = ASSOCSTATUS_DISCONNECTED;

		consoleClear();

		Wifi_AccessPoint* ap = findAP();

		iprintf("Connecting to %s\n", ap->ssid);

		//this tells the wifi lib to use dhcp for everything
		Wifi_SetIP(0,0,0,0,0);	

		Wifi_ConnectAP(ap, WEPMODE_NONE, 0, 0);

		while(status != ASSOCSTATUS_ASSOCIATED && status != ASSOCSTATUS_CANNOTCONNECT)
		{
			int oldStatus = status;

			status = Wifi_AssocStatus();

			iprintf("%s", oldStatus != status ? ASSOCSTATUS_STRINGS[status] : ".");

			scanKeys();

			if(keysDown() & KEY_B) break;
			
			swiWaitForVBlank();
		}

		consoleClear();
		consoleSetWindow(NULL, 0,0,32,10);

		char url[256];

		if(status == ASSOCSTATUS_ASSOCIATED) while(1)
		{
			u32 ip = Wifi_GetIP();

			iprintf("ip: [%i.%i.%i.%i]", (ip ) & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF);

			scanf("%s", url);

			struct hostent *host = gethostbyname(url);

			if(host)
				iprintf("IP (%s) : %s\n",  url, inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));
			else
				iprintf("Could not resolve\n");

			scanKeys();

			if(keysDown() & KEY_B) break;

			swiWaitForVBlank();
		}
	}
	return 0;
}
