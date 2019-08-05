// System Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "system.h"

extern bool ShutDown=false;

SystemDriver::SystemDriver()
{
  ShutDown=false;
  // Video and Input Init
#if 0
  allegro_init();
  install_keyboard();
  install_mouse();
//  if (set_gfx_mode(GFX_AUTODETECT,1024,768,0,0))
    if (set_gfx_mode(GFX_AUTODETECT,800,600,0,0))
      if (set_gfx_mode(GFX_AUTODETECT,640,480,0,0))
        if (set_gfx_mode(GFX_AUTODETECT,320,240,0,0))
          set_gfx_mode(GFX_AUTODETECT,320,200,0,0);
  video=screen;
  video=create_bitmap(SCREEN_W,SCREEN_H);
#endif
}

SystemDriver::~SystemDriver()
{
}

void SystemDriver::Initialize()
{
}

void SystemDriver::InitApp()
{
}

void SystemDriver::Loop()
{
  while (!ShutDown)
    {
      NextFrame();
    }
}

void SystemDriver::NextFrame()
{
#if 0
   clear(video);
#endif
}

void SystemDriver::FinishDraw()
{
#if 0
   blit(video,screen,0,0,0,0,SCREEN_W,SCREEN_H);
#endif
}

