// Entity Class for Trohs 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "trohs3d.h"
#include "engine/vector3d.h"
#include "engine/polyset.h"

class Entity;
class Vector3d;
class Polygon3d;
class Polygon2d;
class ReversibleTransform;
class Camera;
class Clipper;

// At Lowest Level, an entity is like a sector
class Entity:public PolySet
{
  public:
    char id[80];
    Vector3d center;

    Entity(char *name);

    // Update polygons camera verts with Camera
    virtual void CamUpdate ( Camera *cam);

    virtual void DrawOnePolygon( Polygon3d *po, Polygon2d *p);

    virtual void Draw( Camera *cam, Clipper *clipper);

  protected:
};

#endif
