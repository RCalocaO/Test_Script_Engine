// 3d Polygon Class
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __POLYGON3D_H__
#define __POLYGON3D_H__

#include "../trohs3d.h"
class Vector2d;
class Vector3d;
class Sector;
class Polygon2d;
class PolySet;
class Camera;

class Polygon3d
{
  public:
    // Display Options
    int color;
    BITMAP *texture;
    int type; // Type is DRAW_(VERTICES,SOLID,WIREFRAME,TEXTUREMAP)
    void SetColor(int c) { color=c; };
    void SetType(int t) { type=t; };

    Polygon3d *next;
    Polygon3d();
    Polygon3d(Polygon3d& poly);
    virtual ~Polygon3d();
    void SetParent(PolySet* s) { poly_set=s; };

    // Add vertex: check if the vertex exists in the containing poly_set
    // yes, then return that index. else add the new vertex.
    int AddVertex ( Vector3d& v);
    int AddVertex (float x, float y, float z);

    // Add vertex using index from poly_set
    int AddVertex (int v);

    // Fills csPolygon2D with a perspective corrected polygon
    // clipped to the view plane (Z=SMALL_Z)
    // If all vertices behind view plane, invisible -> false.
    // also does back-face culling and returns falseif not visible
    bool DoPerspective( Camera& trans,Vector3d* source,
   	  int num_verts,Polygon2d* dest/*,Vector2d* orig_triangle*/);


   // false if polygon is not visible (backface culling, no visible vertices,
   // ...) and 'verts' will be NULL. Otherwise true and 'verts' will point to
   // the new clipped polygon
   bool ZClip(Vector3d* pverts,int& num_verts,bool cw = true);


   bool IntersectSegment( Vector3d& start, Vector3d& end,
                               Vector3d& isect, float* pr=NULL);


  protected:
    // Sector containing actual vertex info
    PolySet *poly_set;
    // Table of indices
    int num_vertices;
    int max_vertices;
    int *index;
};


#endif