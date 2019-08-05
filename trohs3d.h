// Main Include File
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __TROHS3D_H__
#define __TROHS3D_H__

// Useful Macros
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef ABS
#define ABS(x) ((x)<0?-(x):(x))
#endif

#ifndef ABS
#define SIGN(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
#endif

#undef EPSILON
#define EPSILON 0.001				/* Small value */
#undef SMALL_EPSILON
#define SMALL_EPSILON 0.000001			/* Very small value */
#define SMALL_Z .01

// Standard Include Files
#include <stdlib.h>
#include <math.h>
#include <string.h>
#if 0
#include <mem.h>
#include <alloc.h>
#endif

#if 0
// Renderer & Input System
#define alleg_timer_unused
#define alleg_joystick_unused
#define alleg_palette_unused
#define alleg_flic_unused
#define alleg_sound_unused
#define alleg_file_unused
#define alleg_datafile_unused
#define alleg_gui_unused
#include <allegro.h>
#endif

// Global Variables
#if 0
#else
typedef void* BITMAP;
#endif
extern BITMAP *video;
extern bool DEBUG;
extern long StartTime,EndTime,FrameCount;

#include "console.h"

static inline long QRound (float inval)
{
#if 0
	long ret;
  asm ("fistpl %0" : "=m" (ret) : "t" (inval) : "st");
  return ret;
#else
	return (long)inval;
#endif
}

static inline long QRound(double inval)
{
	return (long)inval;
}

// For Polygon Drawing!
#define DRAW_VERTICES   0
#define DRAW_WIREFRAME  1
#define DRAW_SOLID      2
#define DRAW_TEXTUREMAP 3


#endif


