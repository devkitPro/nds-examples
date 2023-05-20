
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

   bool askname = true;

   while(pmMainLoop()) {
      swiWaitForVBlank();
      scanKeys();

      u32 keys = keysDown();
      if (keys & KEY_START) {
         break;
      } else if (keys != 0) {
         askname = true;
      }

      if (askname) {
         char myName[256];

         consoleClear();
         iprintf("What is your name?\n");
         iscanf("%s", myName);
         iprintf("\nHello %s", myName);
         askname = false;
      }
   }

   return 0;
}
