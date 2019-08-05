// Camera
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __POLYGON2D_H__
#define __POLYGON2D_H__

#include "../trohs3d.h"
#include "poly2d.h"
#include "vector3d.h"

class Clipper;
class View;

// 2D polygon (the 2D coordinates are perspective corrected coordinates).
// Source Polygon is converted to 2D using Polygon3D::DoPerspective.
class Polygon2d:public Poly2d
{
  public:
    // Compute the perspective transformation of a 3D vertex and add it to the polygon.
    void AddPerspective(Vector3d& v) { AddPerspective (v.x, v.y, v.z); }
    void AddPerspective(double x,double y,double z);

    // Draw the polygon (wireframe version).
    void DrawWireframe(int color) const;

    // Draw the polygon (vertices version).
    void Draw(int color) const;

    // Draw the polygon (solid color filled version).
    void DrawSolid(int color) const;

    // Draw the polygon (texture mapped version).
    void DrawTexture() const;

    void SetType(int t) { type=t; };

  protected:
    int type;
};

// Factory to create csPolygon2D objects.
class Polygon2DFactory:public Poly2DFactory
{
public:
  // A shared factory that you can use.
  static Polygon2DFactory* SharedFactory();

  // Create a poly2d.
  virtual Poly2d* Create ()
    { Polygon2d* p = new Polygon2d (); return (Poly2d*)p; }
};


#endif
