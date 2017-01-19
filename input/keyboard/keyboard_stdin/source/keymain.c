
#include <nds.h>
#include <stdio.h>


void OnKeyPressed(int key) {
   if(key > 0)
      iprintf("%c", key);
}

int main(void)  {

   consoleDemoInit();

   Keyboard *kbd = 	keyboardDemoInit();

   kbd->OnKeyPressed = OnKeyPressed;

   while(1) {
      char myName[256];

      iprintf("What is your name?\n");

      scanf("%s", myName);

      iprintf("\nHello %s", myName);

      int keys = 0;

      while(!keys) {
         scanKeys();
         keys = keysDown();
         if(keys & KEY_START) exit(0);
      }

      swiWaitForVBlank();
      consoleClear();
   }

   return 0;
}
