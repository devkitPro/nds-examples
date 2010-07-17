#include <nds.h>

#include <stdio.h>


const char* months[12] = {"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"};

const char* languages[8] = {"Japanese", "English", "French", "German", "Italian", "Spanish", "Chinese", "Reserved"};

const char* colors[16] =
{
	"Gray",					"Brown",
	"Red",					"Pink",
	"Orange",				"Yellow",
	"Yellow/Green-ish",		"Green",
	"Dark Green",			"Green/Blue-ish",
	"Light Blue",			"Blue",
	"Dark Blue",			"Dark Purple",
	"Purple",				"Purple/Red-ish"
};


void waitfor(int keys)
{
	scanKeys();
	while((keysDown() & keys) == 0)
	{
		swiWaitForVBlank();
		scanKeys();
	}
}



#define ASCII_MASK 0x00FF
#define UNICODE_MASK 0xFF00

uint convertUnicodeToAscii(char* buffer, const uint bufferLength, const u16* unicode, const uint unicodeLength)
{
	uint length = 0;
	for(; length < unicodeLength && length < bufferLength-1; length++)
	{
		buffer[length] = ((unicode[length] & UNICODE_MASK) == 0 ? (unicode[length] & ASCII_MASK): '?');
	}
	buffer[length] = '\0';
	return length;
}




int main()
{
	consoleDemoInit();


	char asciiname[11];
	convertUnicodeToAscii(asciiname, 11, (u16*)PersonalData->name, PersonalData->nameLen);
	iprintf("name: %i \"%s\"\n", PersonalData->nameLen, asciiname);

	char message[27];
	convertUnicodeToAscii(message, 27, (u16*)PersonalData->message, PersonalData->messageLen);
	iprintf("message: %i \"%s\"\n", PersonalData->messageLen, message);

	iprintf("birthday: %i %s\n", PersonalData->birthDay, months[PersonalData->birthMonth -1]);
	iprintf("alarm: %02i:%02i\n", PersonalData->alarmHour, PersonalData->alarmMinute);

	iprintf("language: %s\n", (PersonalData->language < 7 ? languages[PersonalData->language] : languages[7]));
	iprintf("menu color/theme: %s\n", colors[PersonalData->theme]);

	iprintf("\nDS (light) only\n");
	iprintf("gba games on: %s screen\n", (PersonalData->gbaScreen ? "Bottom": "Top"));
	iprintf("autoboot cardridge: %s\n", (PersonalData->autoMode ? "yes": "no"));


	waitfor(KEY_A);

	return 0;
}
