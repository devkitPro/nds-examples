/*---------------------------------------------------------------------------------

Simple wifi demo to locate and connect to an ap
-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdlib.h>
#include <stdio.h>
#include <wfc.h>
#include <dswifi9.h>

#include <sys/types.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <netdb.h>

#include "wifiicon.h"

//---------------------------------------------------------------------------------
static const char* const signalStrength[] = {
//---------------------------------------------------------------------------------
	"[   ]",
	"[.  ]",
	"[.i ]",
	"[.iI]",
};

//---------------------------------------------------------------------------------
static const char* const authTypes[] = {
//---------------------------------------------------------------------------------
	"Open",
	"WEP",
	"WEP",
	"WEP",
	"WPA-PSK-TKIP",
	"WPA-PSK-AES",
	"WPA2-PSK-TKIP",
	"WPA2-PSK-AES",
};

//---------------------------------------------------------------------------------
static const char* const connStatus[] = {
//---------------------------------------------------------------------------------
	"Disconnected :(",
	"Searching...",
	"Associating...",
	"Obtaining IP address...",
	"Connected!",
};

//---------------------------------------------------------------------------------
static inline WlanBssAuthType authMaskToType(unsigned mask) {
//---------------------------------------------------------------------------------
	return mask ? (WlanBssAuthType)(31-__builtin_clz(mask)) : WlanBssAuthType_Open;
}

//---------------------------------------------------------------------------------
static WlanBssDesc* findAP(void) {
//---------------------------------------------------------------------------------
	int selected = 0;
	int i;
	int displaytop = 0;
	unsigned count = 0;
	WlanBssDesc* aplist = NULL;
	WlanBssDesc* apselected = NULL;

	static const WlanBssScanFilter filter = {
		.channel_mask = UINT32_MAX,
		.target_bssid = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	};

_rescan:
	if (!wfcBeginScan(&filter)) {
		return NULL;
	}

	iprintf("Scanning APs...\n");

	while (pmMainLoop() && !(aplist = wfcGetScanBssList(&count))) {
		swiWaitForVBlank();
		scanKeys();

		if (keysDown() & KEY_START) exit(0);
	}

	if (!aplist || !count) {
		iprintf("No APs detected\n");
		return NULL;
	}

	while (pmMainLoop()) {
		swiWaitForVBlank();
		scanKeys();

		u32 pressed = keysDown();
		if (pressed & KEY_START) exit(0);
		if (pressed & KEY_A) {
			apselected = &aplist[selected];
			break;
		}

		consoleClear();

		if (pressed & KEY_R) {
			goto _rescan;
		}

		iprintf("%u APs detected (R = rescan)\n\n", count);

		int displayend = displaytop + 10;
		if (displayend > count) displayend = count;

		//display the APs to the user
		for (i = displaytop; i < displayend; i++) {
			WlanBssDesc* ap = &aplist[i];

			// display the name of the AP
			iprintf("%s%.29s\n  %s Type:%s\n",
				i == selected ? "*" : " ",
				ap->ssid_len ? ap->ssid : "-- Hidden SSID --",
				signalStrength[wlanCalcSignalStrength(ap->rssi)],
				authTypes[authMaskToType(ap->auth_mask)]);
		}

		//move the selection asterisk
		if (pressed & KEY_UP) {
			selected--;
			if (selected < 0) {
				selected = 0;
			}

			if (selected < displaytop) {
				displaytop = selected;
			}
		}

		if (pressed & KEY_DOWN) {
			selected++;
			if (selected >= count) {
				selected = count - 1;
			}

			displaytop = selected - 9;
			if (displaytop < 0) {
				displaytop = 0;
			}
		}
	}

	return apselected;
}

//---------------------------------------------------------------------------------
static void keyPressed(int c){
//---------------------------------------------------------------------------------
	if(c > 0) iprintf("%c",c);
}

//---------------------------------------------------------------------------------
static bool die(bool showMsg) {
//---------------------------------------------------------------------------------
	showMsg = showMsg && pmMainLoop();

	if (showMsg) {
		iprintf("Press A to try again, B to quit\n");
	}

	while (pmMainLoop()) {
		swiWaitForVBlank();
		scanKeys();

		u32 pressed = keysDown();
		if (pressed & KEY_A) return true;
		if (pressed & (KEY_B|KEY_START)) break;
	}

	return false;
}

//---------------------------------------------------------------------------------
static void wifiSignalIsr(void) {
//---------------------------------------------------------------------------------
	unsigned level = wlmgrGetSignalStrength();
	bool is_active = wlmgrGetState() >= WlMgrState_Associating;

	oamSetGfx(&oamSub, 0, SpriteSize_16x16, SpriteColorFormat_16Color, &SPRITE_GFX_SUB[level*wifiiconTilesLen/(4*sizeof(u16))]);
	oamSetHidden(&oamSub, 0, !is_active);
	oamUpdate(&oamSub);
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();

	vramSetBankD(VRAM_D_SUB_SPRITE);
	oamInit(&oamSub, SpriteMapping_Bmp_1D_128, false);

	dmaCopy(wifiiconPal, SPRITE_PALETTE_SUB, wifiiconPalLen);
	dmaCopy(wifiiconTiles, SPRITE_GFX_SUB, wifiiconTilesLen);

	oamSet(&oamSub, 0, 256-16, 0, 0, 0, SpriteSize_16x16, SpriteColorFormat_16Color, SPRITE_GFX_SUB, -1, false, false, false, false, false);
	oamSetHidden(&oamSub, 0, true);
	irqSet(IRQ_VBLANK, wifiSignalIsr);

	Keyboard* kb = keyboardDemoInit();
	kb->OnKeyPressed = keyPressed;

	if (!Wifi_InitDefault(false)) {
		iprintf("Wifi init fail\n");
		die(false);
		return 0;
	}

	char instr[64];
	unsigned inlen;

	// Auth data must be in main RAM, not DTCM stack
	static WlanAuthData auth;

	do {
		consoleClear();
		consoleSetWindow(NULL, 0,0,32,24);

		WlanBssDesc* ap = findAP();
		if (!ap) continue;

		consoleClear();
		consoleSetWindow(NULL, 0,0,32,10);

		if (!ap->ssid_len) {
			iprintf("Enter hidden SSID name\n");
			bool ok;
			do {
				if (!fgets(instr, sizeof(instr), stdin)) {
					exit(0);
				}

				instr[strcspn(instr, "\n")] = 0;
				inlen = strlen(instr);

				ok = inlen && inlen <= WLAN_MAX_SSID_LEN;
				if (!ok) {
					iprintf("Invalid SSID\n");
				}
			} while (!ok);

			memcpy(ap->ssid, instr, inlen);
			ap->ssid_len = inlen;
		}

		iprintf("Connecting to %.*s\n", ap->ssid_len, ap->ssid);
		ap->auth_type = authMaskToType(ap->auth_mask);
		memset(&auth, 0, sizeof(auth));

		if (ap->auth_type != WlanBssAuthType_Open) {
			iprintf("Enter %s key\n", authTypes[ap->auth_type]);
			bool ok;
			do {
				if (!fgets(instr, sizeof(instr), stdin)) {
					exit(0);
				}

				instr[strcspn(instr, "\n")] = 0;
				inlen = strlen(instr);

				ok = true;
				if (ap->auth_type < WlanBssAuthType_WPA_PSK_TKIP) {
					if (inlen == WLAN_WEP_40_LEN) {
						ap->auth_type = WlanBssAuthType_WEP_40;
					} else if (inlen == WLAN_WEP_104_LEN) {
						ap->auth_type = WlanBssAuthType_WEP_104;
					} else if (inlen == WLAN_WEP_128_LEN) {
						ap->auth_type = WlanBssAuthType_WEP_128;
					} else {
						ok = false;
					}
				} else if (inlen < 1 || inlen >= WLAN_WPA_PSK_LEN) {
					ok = false;
				}

				if (!ok) {
					iprintf("Invalid key!\n");
				}
			} while (!ok);

			if (ap->auth_type < WlanBssAuthType_WPA_PSK_TKIP) {
				memcpy(auth.wep_key, instr, inlen);
			} else {
				iprintf("Deriving PMK, please wait\n");
				wfcDeriveWpaKey(&auth, ap->ssid, ap->ssid_len, instr, inlen);
			}
		}

		if (!wfcBeginConnect(ap, &auth)) {
			continue;
		}

		bool is_connect = false;
		while (pmMainLoop()) {
			swiWaitForVBlank();
			scanKeys();

			if (keysDown() & KEY_START) exit(0);

			int status = Wifi_AssocStatus();

			consoleClear();
			iprintf("%s\n", connStatus[status]);

			is_connect = status == ASSOCSTATUS_ASSOCIATED;
			if (is_connect || status == ASSOCSTATUS_DISCONNECTED) break;
		}

		if (is_connect) {
			unsigned ip = Wifi_GetIP();
			iprintf("Our IP: %u.%u.%u.%u\n", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, (ip >> 24) & 0xff);

			for (;;) {
				iprintf("Enter domain name\n");
				if (!fgets(instr, sizeof(instr), stdin)) {
					break;
				}

				instr[strcspn(instr, "\n")] = 0;
				inlen = strlen(instr);
				if (!inlen) break;

				struct hostent* host = gethostbyname(instr);
				if (host) {
					iprintf("Domain IP: %s\n", inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));
				} else {
					iprintf("Could not resolve domain\n");
				}
			}

			Wifi_DisconnectAP();
		}

	} while (die(true));

	return 0;
}
