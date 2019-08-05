// Polygon Clippers
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __CLIPPER_H__
#define __CLIPPER_H__


class Clipper;

#include "vector2d.h"
#include "poly2d.h"
#include "boundbox.h"
#include "../trohs3d.h"
#include "polypool.h"

// Ancestor, because a box clipping is a special case of a polygon clipper!
class Clipper
{
public:
  // This variable holds a pool for 2D polygons as used by the clipper.
  static Poly2DPool polypool;
  // Clip a set of 2D points and return in 'dest_poly'.
  // dest_poly  must be big enough to hold the clipped polygon.
  // False if polygon is not visible (clipped away).
  virtual bool Clip (Vector2d *Polygon, Vector2d* dest_poly, int Count,
  	int &OutCount) const =0;

  // Clip a set of 2D points and return them in the same array.
  // On input MaxCount contains number of elements that Output can hold.
  // On output Count is set to number of vertices in output polygon.
  virtual bool Clip (Vector2d *Polygon, int &Count, int MaxCount, 
                     BoundBox *BoundingBox) const =0;

  // Return true if given point is inside (or on bound) of clipper polygon.
  virtual bool IsInside (double x, double y) const=0;

  // Return number of vertices for this clipper polygon.
  virtual int GetNumVertices() const=0;

  // Return vertex at index for this clipper polygon.
  virtual const Vector2d GetVertex(int i) const =0;
};

// A Rectangular Object(As screen)
class BoxClipper : public Clipper
{
  BoundBox region;
  // This variable holds a pool for 2D polygons as used by the clipper.
  static Poly2DPool polypool;

public:
  // Initializes the clipper object to the given bounding rectangle
  BoxClipper(const BoundBox& b) : region(b) {}

  // Initializes the clipper object to a rectangle with the given coords.
  BoxClipper(double x1,double y1,double x2,double y2):region((float)x1, (float)y1, (float)x2, (float)y2) {}

  // Clip a to dest_poly.
  virtual bool Clip (Vector2d *Polygon,Vector2d* dest_poly, int Count,
  	int &OutCount) const override;

  // Clip a set of 2D points and return them in the same array.
  // On input MaxCount contains number of elements that Output can hold.
  // On output Count is set to number of vertices in output polygon.
  virtual bool Clip (Vector2d *Polygon, int &Count, int MaxCount,
                     BoundBox *BoundingBox) const override;


  // Return true if given point is inside (or on bound) of clipper polygon.
  virtual bool IsInside (double x,double y)  const override { return region.In ((float)x, (float)y); }

  // Return number of vertices for this clipper polygon.
  virtual int GetNumVertices ()  const override { return 4; }

  // Return vertex at index for this clipper polygon.
  virtual const Vector2d GetVertex (int i) const override
  {
    switch (i)
    {
      case 0: return Vector2d(region.MinX(),region.MinY());
      case 1: return Vector2d(region.MaxX(),region.MinY());
      case 2: return Vector2d(region.MaxX(),region.MaxY());
      default: return Vector2d(region.MinX(),region.MaxY());
    }
  }
};

// Clips any convex polygon with another convex polygon
class PolygonClipper : public Clipper
{
  // This variable holds a pool for 2D polygons as used by the clipper.
//  static Poly2DPool polypool;
  /// Private structure for keeping pre-calculated some data
  struct SegData
  {
    double dx, dy;
  };
  // Equation for all edges of clipping polygon
  SegData *ClipData;
  // Clipper polygon itself
  Vector2d *ClipPoly;
  // A pointer to the pooled polygon (so that we can free it later).
  Poly2d* ClipPoly2D;
  // Number of vertices in clipper polygon
  int ClipPolyVertices;
  // Clipping polygon bounding box
  BoundBox ClipBox;

public:
  // Create a polygon clipper object from a set of 2D vectors.
  PolygonClipper (Poly2d *Clipper, bool copy = false);

  // Destroy the polygon clipper object.
  virtual ~PolygonClipper ();

  // Clip to dest_poly.
  virtual bool Clip (Vector2d *Polygon,Vector2d* dest_poly, int Count,
	  int &OutCount) const override;

  // Clip a set of 2D points and return them in the same array.
  // On input MaxCount contains number of elements that Output can hold.
  // On output Count is set to number of vertices in output polygon.
  virtual bool Clip (Vector2d *Polygon, int &Count, int MaxCount,
                     BoundBox *BoundingBox) const override;

  // Return true if given point is inside (or on bound) of clipper polygon.
  virtual bool IsInside(double x,double y) const override;

  // Return number of vertices for this clipper polygon.
  virtual int GetNumVertices () const override { return ClipPolyVertices; }

  // Return vertex at index for this clipper polygon.
  virtual const Vector2d GetVertex(int i) const override { return ClipPoly[i]; }

};

#endif

