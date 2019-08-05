// Script Parser for Trohs 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#ifndef __SCRIPTPARSER_H__
#define __SCRIPTPARSER_H__

#include "../trohs3d.h"

class QuadList;

// Returns a QuadList if compilation succesful of script
// NULL if errors
extern QuadList* ParseScript(const char *scriptfilename);

extern int ConstInt,ConstReal;

#endif
