// Polygon Set Class for 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "polyset.h"
#include "polygon3d.h"
#include "polygon2d.h"
#include "poly2d.h"
#include "../world.h"
#include "camera.h"
#include "sector.h"
#include "polypool.h"
#include "vector3d.h"
#include "clipper.h"

PolySet::PolySet()
{
   //cout << "\tCreating PolySet..." << endl;
   next=NULL;
   num_vertices=max_vertices=num_polygon=0;
   worldv=camerav=NULL;
   polygons=NULL;
}

PolySet::~PolySet()
{
#if 0
   cout << "\tDestroying PolySet..." << endl;
#endif
   for (int i=0; i<num_polygon; i++)
     delete polygons[i];
   if (polygons) delete [] polygons;
   if (worldv) delete [] worldv;
   if (camerav) delete [] camerav;
   num_vertices=num_polygon=0;
   polygons=NULL;
   worldv=camerav=NULL;
}

int PolySet::AddVertex(float x, float y, float z)
{
  if (!worldv)
  {
    max_vertices = 10;
    worldv = new Vector3d[max_vertices];
    camerav = new Vector3d[max_vertices];
  }
  while (num_vertices >= max_vertices)
  {
    max_vertices += 10;
    Vector3d* new_wor_verts = new Vector3d[max_vertices];
    Vector3d* new_obj_verts = new Vector3d[max_vertices];
    memcpy (new_wor_verts, worldv, sizeof (Vector3d)*num_vertices);
    memcpy (new_obj_verts, camerav, sizeof (Vector3d)*num_vertices);

    delete [] worldv;
    delete [] camerav;

    worldv = new_wor_verts;
    camerav = new_obj_verts;
  }

  // By default all vertices are set with the same object space and world space.
  worldv[num_vertices].Set (x, y, z);
  num_vertices++;
  return num_vertices-1;
}

int PolySet::AddVertexSmart (float x, float y, float z)
{
  if (!worldv) { AddVertex (x, y, z); return 0; }
  int i;
  for (i = 0 ; i < num_vertices ; i++)
    if (ABS (x-worldv[i].x) < SMALL_EPSILON &&
	ABS (y-worldv[i].y) < SMALL_EPSILON &&
	ABS (z-worldv[i].z) < SMALL_EPSILON)
    {
      return i;
    }
  AddVertex (x, y, z);
  return num_vertices-1;
}

Polygon3d* PolySet::NewPolygon()
{
  Polygon3d* p = new Polygon3d();
//  p->SetPolySet (sector);
  AddPolygon (p);
  return p;
}

void PolySet::AddPolygon(Polygon3d* poly)
{
  if (!polygons)
  {
    max_polygon = 6;
    polygons = new Polygon3d* [max_polygon];
  }
  while (num_polygon >= max_polygon)
  {
    max_polygon += 6;
    Polygon3d** new_polygons = new Polygon3d* [max_polygon];
    memcpy (new_polygons, polygons, sizeof (Polygon3d*)*num_polygon);
    delete [] polygons;
    polygons = new_polygons;
  }
  polygons[num_polygon++] = poly;
  ((Polygon3d*)poly)->SetParent (this);
}

void PolySet::Draw( Camera *cam, Clipper *clipper) 
{
  // Draw Polygons
  Polygon3d* p; // Current
  Polygon2d* clip;
  static Vector3d verts[100];
  int num_verts,i;

  Poly2DPool* render_pool = World::current_world->render_pol2d_pool;

  CamUpdate(cam);
  for (i=0; i<num_polygon; i++)
  {
    clip = (Polygon2d*)(render_pool->Alloc ());
    p=(Polygon3d*)polygons[i];
    if (p->ZClip(verts, num_verts))
      if (p->DoPerspective(*cam,verts,num_verts,clip))
        if (clip->ClipAgainst(clipper))
          DrawOnePolygon(p,clip);
    render_pool->Free (clip);
  }

}

// Convert from World->Space Coordinates
void PolySet::CamUpdate( Camera* cam)
{
  for (int i=0; i<num_vertices; i++)
     camerav[i]=cam->World2Camera(worldv[i]);
}


void PolySet::DrawOnePolygon( Polygon3d *po, Polygon2d *p)
{
  // Missing: Check for Portals!
  switch(po->type)
    {
      case DRAW_VERTICES: p->Draw(po->color);
                          break;
      case DRAW_WIREFRAME: p->DrawWireframe(po->color);
                           break;
      case DRAW_SOLID: p->DrawSolid(po->color);
                       break;
      case DRAW_TEXTUREMAP:
                            break;
    };
}

