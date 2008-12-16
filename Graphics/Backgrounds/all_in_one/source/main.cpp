#include <nds.h>

#include <RotBackgrounds.h>
#include <TextBackgrounds.h>
#include <Multilayer.h>

#include <stdio.h>
#include <stdlib.h>



//function prototypes
void HandMadeTiles(void);

void Text256x256(void);
void Text256x512(void);
void Text512x256(void);
void Text512x512(void);
void ExRot128x128(void);
void ExRot256x256(void);
void ExRot512x512(void);
void ExRot1024x1024(void);
void Rot128x128(void);
void Rot256x256(void);
void Rot512x512(void);
void Rot1024x1024(void);
void Bmp8_128x128(void);
void Bmp8_256x256(void);
void Bmp8_512x256(void);
void Bmp8_512x512(void);
void Bmp8_512x1024(void);
void Bmp8_1024x512(void);
void Bmp16_128x128(void);
void Bmp16_512x256(void);
void Bmp16_256x256(void);
void Bmp16_512x512(void);


void scrollText(void);
void scrollRotation(void);
void scrollVertical(void);
void scrollHorizontalText(void);
void scrollHorizontalExRotation(void);
void scroll4wayText(void);
void scroll4wayExRotation(void);

void advMosaic(void);
void advRotating(void);
void advScaling(void);
void advExtendedPalette(void);
void advMultipleLayers(void);


struct Demo
{
	fp go;
	const char* name;
	const char* description;
};

struct Category
{
	const char* name;
	Demo *demos;
	int count;
};

struct Demo basicExamples[] =
{ 
	{HandMadeTiles, "Handmade Text 256x256", "Displays a handmade 256 x 256 Text background"},
	
	{Text256x256, "Text 256x256", "Displays a 256 x 256 Text background"},
	{Text256x512, "Text 256x512", "Displays a 256 x 512 Text background"},
	{Text512x256, "Text 512x256", "Displays a 512 x 256 Text background"},
	{Text512x512, "Text 512x512", "Displays a 512 x 512 Text background"},
	
	{ExRot128x128, "Extended Rotation 128x128", "Displays a 128 x 128 Extended Rotation background"},
	{ExRot256x256, "Extended Rotation 256x256", "Displays a 256 x 256 Extended Rotation background"},
	{ExRot512x512, "Extended Rotation 512x512", "Displays a 512 x 512 Extended Rotation background"},
	{ExRot1024x1024, "Extended Rotation 1024x1024", "Displays a 1024 x 1024 Extended Rotation background"},
	
	{Rot128x128, "Rotation 128x128", "Displays a 256 x 256 Rotation background"},
	{Rot256x256, "Rotation 256x256", "Displays a 256 x 256 Rotation background"},
	{Rot512x512, "Rotation 512x512", "Displays a 512 x 512 Rotation background"},
	{Rot1024x1024, "Rotation 1024x1024", "Displays a 1024 x 1024 Rotation background"},

	
};

struct Demo bitmapExamples[] = 
{
	{Bmp8_128x128, "256 color 128x128", "Displays a 128 x 128 Bitmap background"},
	{Bmp8_256x256, "256 color 256x256", "Displays a 256 x 256 Bitmap background"},
	{Bmp8_512x256, "256 color  512x256", "Displays a 512 x 256 Bitmap background"},
	{Bmp8_512x512, "256 color  512x512", "Displays a 512 x 512 Bitmap background"},
	{Bmp8_512x1024, "256 color  512x1024", "Displays a 512 x 1024 Bitmap background"},
	{Bmp8_1024x512, "256 color  1024x512", "Displays a 1024 x 512 Bitmap background"},
	
	{Bmp16_128x128, "16-bit color 128x128", "Displays a 128 x 128 Bitmap background"},
	{Bmp16_256x256, "16-bit color  256x256", "Displays a 256 x 256 Bitmap background"},
	{Bmp16_512x256, "16-bit color  512x256", "Displays a 512 x 256 Bitmap background"},
	{Bmp16_512x512, "16-bit color  512x512", "Displays a 512 x 512 Bitmap background"},
	

};

struct Demo scrollingExamples[] =
{
	{scrollText, "Text Backgrounds", "Hardware Scrolling of a Text background"},
	{scrollRotation, "Rot Backgrounds", "Hardware Scrolling of a Rotation background"},
	{scrollVertical, "Vertical Scrolling", "Scrolling a large map vertically"},
	{scrollHorizontalText, "Horizontal Scrolling (Text)", "Scrolling a large map horzontally on a text background"},
	{scrollHorizontalExRotation, "Horizontal Scrolling (ExRot)", "Scrolling a large map horzontally on an extended rotation background"},
	{scroll4wayText, "4 Way Scrolling (Text)", "Scrolling a large map 4 ways on a text background"},
	{scroll4wayExRotation, "4 Way Scrolling (Rotation)", "Scrolling a large map 4 ways on a ex rotation background"},

};

struct Demo advancedExamples[] =
{
	{advMosaic, "Mosaic", "A demo of the Mosaic scaling"},
	{advRotating, "Rotation", ""},
	{advScaling, "Scaling", ""},
	{advExtendedPalette, "Extended Palette", ""},
	{advMultipleLayers, "Multiple Text Layers", ""},

};

struct Category categories[] = 
{
	{"Basic", basicExamples, sizeof(basicExamples) / sizeof(Demo)},
	{"Bitmap", bitmapExamples, sizeof(bitmapExamples) / sizeof(Demo)},
	{"Scrolling", scrollingExamples, sizeof(scrollingExamples) / sizeof(Demo)},
	{"Advanced", advancedExamples, sizeof(advancedExamples) / sizeof(Demo)}

};

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int keys;
	

	
	while(1) {
		int selectedCategory = 0;
		int selectedDemo = 0;
		
		bool selected = false;
		
		int catCount = sizeof(categories) / sizeof(Category);
		int demoCount = 0;
		
		videoSetModeSub(MODE_0_2D);
		consoleDemoInit();
		
		while(!selected) {

			scanKeys();
			
			keys = keysDown();
			
			if(keys & KEY_UP) selectedCategory--;
			if(keys & KEY_DOWN) selectedCategory++;
			if(keys & KEY_A) selected = true;
			
			if(selectedCategory < 0) selectedCategory = catCount - 1;
			if(selectedCategory >= catCount) selectedCategory = 0;
			
			swiWaitForVBlank();
			consoleClear();
			for(int ci = 0; ci < catCount; ci++) {
				iprintf("%c%d: %s\n", ci == selectedCategory ? '*' : ' ', ci + 1, categories[ci].name); 		
			}	
		}
		
		selected = false;
		
		demoCount = categories[selectedCategory].count;
		
		while(!selected) {

			scanKeys();
			
			keys = keysDown();
			
			if(keys & KEY_UP) selectedDemo--;
			if(keys & KEY_DOWN) selectedDemo++;
			if(keys & KEY_A) selected = true;
			if(keys & KEY_B) break;
			
			if(selectedDemo < 0) selectedDemo = demoCount - 1;
			if(selectedDemo >= demoCount) selectedDemo = 0;
			
			swiWaitForVBlank();
			consoleClear();

			for(int di = 0; di < demoCount; di++) {
				iprintf("%c%d: %s\n", di == selectedDemo ? '*' : ' ', di + 1, categories[selectedCategory].demos[di].name); 		
			}	
		}
		
		if(selected) {
			consoleClear();
			iprintf("Use arrow keys to scroll\nPress 'B' to exit");
			categories[selectedCategory].demos[selectedDemo].go();
		}
	}
	
}


