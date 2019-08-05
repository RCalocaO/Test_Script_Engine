// Class Tester Entry Point
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#include "simple.h"
#include "world.h"

int main(int,char**)
{
   Simple *Sys=new Simple();
   Sys->world=new World();
   Sys->Initialize();
   Sys->InitApp();
   Sys->Loop();
//   console.Run();
   delete Sys;
   CleanUp(); // Clean HW!
   //readkey();
   // Report FPS

   return 192;
}

