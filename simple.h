// Polygon Set for Trohs 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#ifndef __SIMPLE_H__
#define __SIMPLE_H__

#include "system.h"
#include "engine/vector3d.h"
#include "engine/matrix.h"
class World;
class View;

// Class Derived from System Dependent Driver!
class Simple:public SystemDriver
{
  public:
    World *world;
    View *view;
    Simple();
    void InitApp();
    // Inherited!
    virtual ~Simple();
    virtual void NextFrame();
    void AttendKey();
  protected:
   // Player position, orientation, and velocity
   Vector3d pos,velocity;
   Matrix orient;
   // Camera angles. X and Y are user controllable, Z is not.
   Vector3d angle;
   // Angular velocity: angle_velocity.x is constantly added to angle.x and so on.
   Vector3d angle_velocity;
   // Initial speed of jumping.
   float cfg_jumpspeed;
   // Walk acceleration.
   float cfg_walk_accelerate,cfg_walk_maxspeed;
   // Walk brake deceleration.
   float cfg_walk_brake;
   // Rotate acceleration.
   float cfg_rotate_accelerate;
   // Rotate maximum speed.
   float cfg_rotate_maxspeed;
   // Rotate brake deceleration.
   float cfg_rotate_brake;
   // Look acceleration.
   float cfg_look_accelerate;
   // Body height
   float cfg_body_height,cfg_body_width,cfg_body_depth;
   // Eye offset.
   float cfg_eye_offset;
   // Legs width
   float cfg_legs_width,cfg_legs_depth,cfg_legs_offset;
};

#endif 