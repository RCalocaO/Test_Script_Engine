// Hardware System
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#ifndef __POLYSET_H__
#define __POLYSET_H__

#include "trohs3d.h"

class SystemDriver
{
  public:
    SystemDriver();
    virtual ~SystemDriver();
    void Initialize();
    void InitApp();
    void Loop();
    virtual void NextFrame();
    virtual void FinishDraw();
};

extern void CleanUp();
extern bool ShutDown;

#endif
