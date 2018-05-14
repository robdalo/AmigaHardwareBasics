#include "interrupts.h"
#include "mouse.h"
#include "screen.h"

void MainLoop(void);
void Init(void);
void Finish(void);

int main(int argc, char **argv)
{
   Init();
   MainLoop();
   Finish();

   return 0;
}

void Finish(void)
{
   RestoreScreen();
   RestoreInterrupts();
}

void Init(void)
{
   WaitEndOfFrame();
   WaitEndOfFrame();
   DisableInterrupts();
   InitScreen();
}

void MainLoop(void)
{
   do
   {
      UpdateScreen();
   }
   while 
   (
      LeftMouseDown() != 0
   );
}
