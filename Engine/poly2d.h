// 2d Polygon Class
// USED FOR CLIPPING!
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __POLY2D_H__
#define __POLY2D_H__

#include "../trohs3d.h"
#include "vector2d.h"
#include "boundbox.h"
class Clipper;

// This is a 2d polygon with origin equal as screen upper left corner!
// It is used only for Clipping or Wireframe! For Rendering, use Polygon2d
class Poly2d
{
  protected:
    Vector2d *vertices;
    int num_vertices;
    int max_vertices;
    BoundBox bbox;

  public:

    void Draw(int color);

    Poly2d(int startsize=10);
    Poly2d(Poly2d& copy);
    virtual ~Poly2d();

    // Make the polygon empty!
    void MakeEmpty ();

    // Get the number of vertices.
    int GetNumVertices() { return num_vertices; }

    // Get the array with all vertices.
    Vector2d* GetVertices () { return vertices; }

    // Get the specified vertex.
    Vector2d* GetVertex (int i)
    {
      if (i<0 || i>=num_vertices) return NULL;
      return &vertices[i];
    }

    // Get the specified vertex.
    Vector2d& operator[] (int i)
    { return vertices[i]; }

    // Get the first vertex.
    Vector2d* GetFirst ()
    { if (num_vertices<=0) return NULL;  else return vertices; }

    // Get the last vertex.
    Vector2d* GetLast ()
    { if (num_vertices<=0) return NULL; else return &vertices[num_vertices-1]; }

    // Make room for at least the specified number of vertices.
    void MakeRoom (int new_max);

    // Add a vertex (2D) to the polygon.
    void AddVertex (const Vector2d& v) { AddVertex (v.x, v.y); }

    // Add a vertex (2D) to the polygon.
    void AddVertex (double x, double y);

    // Set all polygon vertices at once. (Copy from v!)
    void SetVertices (Vector2d *v, int num)
    { memcpy (vertices, v, (num_vertices = num) * sizeof (Vector2d)); }

    // Get the bounding box (in 2D space) for this polygon.
    BoundBox& GetBoundingBox () { return bbox; }

  // Return false if the resulting polygon is not visible
  // Will put the resulting clipped polygon in place of the original polygon
  bool ClipAgainst (const Clipper* view);

};

// Auxiliar CLass for creating Poly2d
class Poly2DFactory
{
  public:
    // Create a poly2d.
    virtual Poly2d* Create()
    { Poly2d* p=new Poly2d(); return p; };

    // A shared factory that you can use.
    static Poly2DFactory* SharedFactory();
};

#endif
