// Bounding Box
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __BOUNDBOX_H__
#define __BOUNDBOX_H__

class BoundBox;
#include "../trohs3d.h"

// The maximum value that a coordinate in the bounding box can use (infinity)
#define CS_BOUNDINGBOX_MAXVALUE 1000000000.

/*
 * A bounding box in 2D space.
 * In order to operate correctly, this bounding box assumes that all values
 * entered or compared against lie within the range 
 * (-CS_BOUNDINGBOX_MAXVALUE, CS_BOUNDINGBOX_MAXVALUE).  It is not
 * recommended to use points outside of this range.
*/
class BoundBox
{
protected:
  /// The top-left x coordinate of the bounding box.
  float minx;
  /// The top-left y coordinate of the bounding box.
  float miny;
  /// The bottom-right x coordinate of the bounding box.
  float maxx;
  /// The bottom-right y coordinate of the bounding box.
  float maxy;

public:
  ///
  float MinX () const { return minx; }
  ///
  float MinY () const { return miny; }
  ///
  float MaxX () const { return maxx; }
  ///
  float MaxY () const { return maxy; }

  /// Test if the given coordinate is in this box.
  bool In (float x, float y) const
  {
    if (x < minx || x > maxx) return false;
    if (y < miny || y > maxy) return false;
    return true;
  }

  /// Test if this box overlaps with the given box.
  bool Overlap (const BoundBox& box) const
  {
    if (maxx < box.minx || minx > box.maxx) return false;
    if (maxy < box.miny || miny > box.maxy) return false;
    return true;
  }

  /// Test if this box is empty.
  bool Empty () const
  {
    if (minx > maxx) return true;
    if (miny > maxy) return true;
    return false;
  }

  /// Initialize this box to empty.
  void StartBoundingBox ()
  {
    minx =  CS_BOUNDINGBOX_MAXVALUE;  miny =  CS_BOUNDINGBOX_MAXVALUE;
    maxx = -CS_BOUNDINGBOX_MAXVALUE;  maxy = -CS_BOUNDINGBOX_MAXVALUE;
  }

  /// Initialize this box to one vertex.
  void StartBoundingBox (Vector2d& v)
  {
	  minx = (float)v.x;  miny = (float)v.y;
    maxx = (float)v.x;  maxy = (float)v.y;
  }

  /// Add a new vertex and recalculate the bounding box.
  void AddBoundingVertex (float x, float y)
  {
    if (x < minx) minx = x;  if (x > maxx) maxx = x;
    if (y < miny) miny = y;  if (y > maxy) maxy = y;
  }

  /**
   * Add a new vertex and recalculate the bounding box.
   * This version is a little more optimal. It assumes however
   * that at least one point has been added to the bounding box.
   */
  void AddBoundingVertexSmart (float x, float y)
  {
    if (x < minx) minx = x; else if (x > maxx) maxx = x;
    if (y < miny) miny = y; else if (y > maxy) maxy = y;
  }

  /// Add a new vertex and recalculate the bounding box.
  void AddBoundingVertex (const Vector2d& v)
  {
    if (v.x < minx) minx = (float)v.x;  if (v.x > maxx) maxx = (float)v.x;
    if (v.y < miny) miny = (float)v.y;  if (v.y > maxy) maxy = (float)v.y;
  }

  /// Add a new vertex and recalculate the bounding box.
  void AddBoundingVertexSmart (const Vector2d& v)
  {
    if (v.x < minx) minx = (float)v.x; else if (v.x > maxx) maxx = (float)v.x;
    if (v.y < miny) miny = (float)v.y; else if (v.y > maxy) maxy = (float)v.y;
  }

  /// Initialize this box to empty.
  BoundBox () : minx( CS_BOUNDINGBOX_MAXVALUE), miny( CS_BOUNDINGBOX_MAXVALUE),
	     maxx(-CS_BOUNDINGBOX_MAXVALUE), maxy(-CS_BOUNDINGBOX_MAXVALUE) {}

  /// Initialize this box with one point.
  BoundBox (const Vector2d& v) : minx((float)v.x), miny((float)v.y), maxx((float)v.x), maxy((float)v.y) {}

  /// Initialize this box with the given values.
  BoundBox (float x1, float y1, float x2, float y2) :
    minx(x1), miny(y1), maxx(x2), maxy(y2)
  { if (Empty ()) StartBoundingBox (); }

  /// Sets the bounds of the box with the given values.
  void Set (float x1, float y1, float x2, float y2)
  {
    if (x1>x2 || y1>y2) StartBoundingBox();
    else { minx = x1;  miny = y1;  maxx = x2;  maxy = y2; }
  }

  /// Compute the union of two bounding boxes.
  BoundBox& operator+= (const BoundBox& box)
  {
    if (box.minx < minx) minx = box.minx;
    if (box.miny < miny) miny = box.miny;
    if (box.maxx > maxx) maxx = box.maxx;
    if (box.maxy > maxy) maxy = box.maxy;
    return *this;
  }

  /// Compute the union of a point with this bounding box.
  BoundBox& operator+= (const Vector2d& point)
  {
    if (point.x < minx) minx = (float)point.x;
    if (point.x > maxx) maxx = (float)point.x;
    if (point.y < miny) miny = (float)point.y;
    if (point.y > maxy) maxy = (float)point.y;
    return *this;
  }

  /// Compute the intersection of two bounding boxes.
  BoundBox& operator*= (const BoundBox& box)
  {
    if (box.minx > minx) minx = box.minx;
    if (box.miny > miny) miny = box.miny;
    if (box.maxx < maxx) maxx = box.maxx;
    if (box.maxy < maxy) maxy = box.maxy;
    return *this;
  }

  /// Compute the union of two bounding boxes.
  friend BoundBox operator+ (const BoundBox& box1, const BoundBox& box2)
  {
    return BoundBox( MIN(box1.minx,box2.minx), MIN(box1.miny,box2.miny),
                  MAX(box1.maxx,box2.maxx), MAX(box1.maxy,box2.maxy));
  }

  /// Compute the union of a bounding box and a point.
  friend BoundBox operator+ (const BoundBox& box, const Vector2d& point)
  {
    return BoundBox((float)MIN(box.minx,point.x), (float)MIN(box.miny,point.y),
		(float)MAX(box.maxx,point.x), (float)MAX(box.maxy,point.y) );
  }

  /// Compute the intersection of two bounding boxes.
  friend BoundBox operator* (const BoundBox& box1, const BoundBox& box2)
  {
    return BoundBox( MAX(box1.minx,box2.minx), MAX(box1.miny,box2.miny),
                     MIN(box1.maxx,box2.maxx), MIN(box1.maxy,box2.maxy) );
  }

  /// Tests if two bounding boxes are equal.
  friend bool operator== (const BoundBox& box1, const BoundBox& box2)
  {
    return ( (box1.minx == box2.minx) && (box1.miny == box2.miny) &&
             (box1.maxx == box2.maxx) && (box1.maxy == box2.maxy) );
  }

  /// Tests if two bounding boxes are unequal.
  friend bool operator!= (const BoundBox& box1, const BoundBox& box2)
  {
    return ( (box1.minx != box2.minx) || (box1.miny != box2.miny) ||
             (box1.maxx != box2.maxx) || (box1.maxy != box2.maxy) );
  }

  /// Tests if box1 is a subset of box2.
  friend bool operator< (const BoundBox& box1, const BoundBox& box2)
  {
    return ( (box1.minx >= box2.minx) && (box1.miny >= box2.miny) &&
             (box1.maxx <= box2.maxx) && (box1.maxy <= box2.maxy) );
  }

  /// Tests if box1 is a superset of box2.
  friend bool operator> (const BoundBox& box1, const BoundBox& box2)
  {
    return ( (box2.minx >= box1.minx) && (box2.miny >= box1.miny) &&
             (box2.maxx <= box1.maxx) && (box2.maxy <= box1.maxy) );
  }

  /// Tests if a point is contained in a box.
  friend bool operator< (const Vector2d& point, const BoundBox& box)
  {
    return ( (point.x >= box.minx) && (point.x <= box.maxx) &&
             (point.y >= box.miny) && (point.y <= box.maxy) );
  }

};


#endif

