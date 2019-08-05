// Polygon Set
// Ancestor of PolySet & Entity
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __POLYSET_H__
#define __POLYSET_H__

#include "../trohs3d.h"
#include "vector3d.h"

class Vector3d;
class Polygon3d;
class Polygon2d;
class ReversibleTransform;
class Camera;
class Clipper;
class Sector;
class World;

class PolySet
{
  public:
    friend class Polygon3d;
    // For Linked List of PolySets
    PolySet *next;
    PolySet();
    ~PolySet();

    // Add a Vertex to List
    int AddVertex( Vector3d& v)
      { return AddVertex((float)v.x, (float)v.y, (float)v.z); }
    int AddVertex(float x, float y, float z);

    // Add a vertex but first check if there is already a vertex close
    int AddVertexSmart ( Vector3d& v)
       { return AddVertexSmart ((float)v.x, (float)v.y, (float)v.z); }
    int AddVertexSmart (float x, float y, float z);

    // Return the world space vector for the vertex.
    Vector3d& WorldVertex(int idx) { return worldv[idx]; }

    // Return the camera space vector for the vertex.
    Vector3d& CameraVertex(int idx) { return camerav[idx]; }

    // Create a new Polygon and add it to PolySet
    Polygon3d* NewPolygon();

    virtual void Draw( Camera *cam, Clipper *clipper) ;

    // Update polygons camera verts with Camera
    virtual void CamUpdate ( Camera *cam);

    virtual void DrawOnePolygon( Polygon3d *po, Polygon2d *p) ;

  protected:
    int max_vertices;
    int num_vertices;
    // (World Space)Original List of Vertices
    Vector3d *worldv;
    // Camera Space
    Vector3d *camerav;
    // Polygon List
    int num_polygon;
    int max_polygon;
    Polygon3d **polygons;
    void AddPolygon(Polygon3d* spoly);
};

#endif