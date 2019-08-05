// Entity Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "engine/polyset.h"
#include "entity.h"
#include "engine/polygon3d.h"
#include "engine/polygon2d.h"
#include "engine/vector3d.h"
#include "engine/clipper.h"
#include "engine/camera.h"
#include "world.h"


Entity::Entity(char *name):PolySet()
{
#if 0
   cout << "\tCreating Entity..." << endl;
#endif
   strcpy(id,name);
   next=NULL;
   num_vertices=max_vertices=num_polygon=0;
   worldv=camerav=NULL;
   polygons=NULL;
   center=Vector3d(0,0,0);
}

// Convert from World->Space Coordinates. Add center to vertices!
void Entity::CamUpdate( Camera* cam)
{
  for (int i=0; i<num_vertices; i++)
     camerav[i]=cam->World2Camera(worldv[i]+center);
}


void Entity::Draw( Camera *cam, Clipper *clipper)
{
  // Draw Polygons
  Polygon3d* p; // Current
  Polygon2d* clip;
  static Vector3d verts[100];
  int num_verts,i;

  Poly2DPool* render_pool = World::current_world->render_pol2d_pool;

  // ADD BACKFACE REMOVAL!!!

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

void Entity::DrawOnePolygon( Polygon3d *po, Polygon2d *p)
{
  PolySet::DrawOnePolygon(po,p);
}

