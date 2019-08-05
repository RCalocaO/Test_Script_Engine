// World Parser
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __WORLDPARSER_H__
#define __WORLDPARSER_H__

class World;
class View;

// This function loads a world description from the file f and creates it.
// Assumes World was already Created and Initialized!
bool LoadWorldFromFile(World *w,View *v,const char *filename);


#endif
