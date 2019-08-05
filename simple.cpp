// Simple Test Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "simple.h"

#include "engine/polyset.h"
//#include "sector.h"
#include "world.h"
#include "engine/view.h"
#include "engine/camera.h"

#include "WorldParser/worldparser.h"
#include "ScriptParser/ScriptParser.h"
#include "ScriptParser/quad.h"
#include "ScriptParser/proc_dir.h"
#include "VirtualMachine/virtualmachine.h"

#include <stdio.h>
#include <time.h>

Simple::Simple()
{
  world=NULL;
  view=NULL;
}

Simple::~Simple()
{
  EndTime=clock();
  if (ICode)
    delete ICode;
  if (SymTable)
    delete SymTable;
  delete world;
  delete view;
}

void Simple::AttendKey()
{
#if 0
  double speed=M_PI_2/30;
  double walkspeed=2.0;
  // Important Keys
  if (key[KEY_ESC]) ShutDown=true;
  if ((key[KEY_LSHIFT]) || (key[KEY_RSHIFT]))
         {
           speed*=2.0;
           walkspeed*=3.5;
         }
  // Mouse Move?
  int dx,dy;
  get_mouse_mickeys(&dx,&dy);
  if (dx | dy)
         {
           view->GetCamera()->RotateWorld(VEC_ROT_RIGHT,(double)dx/250.0);
           view->GetCamera()->Rotate(VEC_TILT_DOWN,(double)dy/250.0);
         }

  // Movement/Command Keys
  if (key[KEY_TAB]) console.Run();
       else if (key[KEY_RIGHT])
        if (key[KEY_ALT])
         view->GetCamera()->Move(VEC_RIGHT*walkspeed);
        else
         view->GetCamera()->RotateWorld(VEC_ROT_RIGHT,speed);
       else if (key[KEY_LEFT])
        if (key[KEY_ALT])
         view->GetCamera()->Move(VEC_LEFT*walkspeed);
        else
         view->GetCamera()->RotateWorld(VEC_ROT_LEFT,speed);
       else if (key[KEY_PGUP]) view->GetCamera()->Rotate(VEC_TILT_DOWN,speed);
       else if (key[KEY_PGDN]) view->GetCamera()->Rotate(VEC_TILT_UP,speed);
       else if (key[KEY_UP]) view->GetCamera()->Move(VEC_FORWARD*walkspeed);
       else if (key[KEY_DOWN]) view->GetCamera()->Move(VEC_BACKWARD*walkspeed);
#endif
}

void Simple::NextFrame()
{
  // Need to call ancestor for init video!
  SystemDriver::NextFrame();
  // Attend Keyboard Events
//  if (keypressed())
    AttendKey();

  // Draw World, process script & draw CrossHair
  view->Draw();

  Trohs3dVM.Step();
  Trohs3dVM.Step();

#if 0
  vline(video,SCREEN_W/2,SCREEN_H/2-3,SCREEN_H/2+3,15);
  hline(video,SCREEN_W/2-3,SCREEN_H/2,SCREEN_W/2+3,15);
#endif

/*
  // Pos info
  Vector3d zx=view->GetCamera()->GetW2CTranslation();
  char tmp[200];
  sprintf(tmp,"Position [%f %f %f]\n",zx.x,zx.y,zx.z);
  messages << tmp;
  messages >> video;
  // Rotation Info
  messages << tmp;
  Matrix zxx=view->GetCamera()->GetW2C();
  sprintf(tmp,"[%f %f %f]\n",zxx.m11,zxx.m12,zxx.m13);
  messages << tmp;
  sprintf(tmp,"[%f %f %f]\n",zxx.m21,zxx.m22,zxx.m23);
  messages << tmp;
  sprintf(tmp,"[%f %f %f]\n",zxx.m31,zxx.m32,zxx.m33);
  messages << tmp;
*/
  messages >> video;

  // Flush to Screen
  SystemDriver::FinishDraw();
  FrameCount++;
}

void Simple::InitApp()
{
  messages.MaxLines=4;
#if 0
  cout << "Simple Trohs3d Application" << endl;
  cout << "(C) 1999 by R. Caloca" << endl;
  cout << "--------------------------" << endl;
  cout << endl;
  cout << "Initializing..." << endl;
#endif
  // Prepare World Parameters
  world->Initialize();
  // Reset the Virtual Machine
  Trohs3dVM.Reset();

  // Make a View Structure
  view=new View(world);

  // Load the World
  const char* WorldFilename = "basicworld.t3d";
  if (__argc > 1)
  {
	  WorldFilename = __argv[1];
  }
  LoadWorldFromFile(world,view, WorldFilename);

  // Load Script ICode QuadList* and load into VM
  const char* ScriptFilename = "basicscript.t3d";
  if (__argc > 2)
  {
	  ScriptFilename = __argv[2];
  }
  ParseScript(ScriptFilename);
  Trohs3dVM.Load();

  // Set Clipping Window
#if 0
  view->SetRectangle(2,2,SCREEN_W-2,SCREEN_H-2);
#endif

  // FPS
  StartTime=clock();
}


