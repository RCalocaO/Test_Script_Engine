// Sector Class for Trohs 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#ifndef __SECTOR_H__
#define __SECTOR_H__

class PolySet;

#include "polyset.h"
#include "../trohs3d.h"
#include "vector3d.h"

class Sector;
class Vector3d;
class Polygon3d;
class Polygon2d;
class ReversibleTransform;
class Camera;
class Clipper;

// At Lowest Level, a sector is nothing but a Set of Polygon3d!
class Sector:public PolySet
{
  public:
    Sector();
    //~Sector();

    Sector* FollowSegment(ReversibleTransform& t,Vector3d& new_position);
    Polygon3d* IntersectSegment(const Vector3d& start,const Vector3d& end,
                                      Vector3d& isect, float* pr=NULL);

  protected:
};

#endif