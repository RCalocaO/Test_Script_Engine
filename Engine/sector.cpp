// Sector Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "../trohs3d.h"
#include "sector.h"
#include "../entity.h"
#include "polygon3d.h"
#include "polygon2d.h"
#include "poly2d.h"
#include "camera.h"
#include "../world.h"
#include "vector3d.h"
#include "clipper.h"


Sector::Sector():PolySet()
{
#if 0
   cout << "\tCreating Sector..." << endl;
#endif
   next=NULL;
   num_vertices=max_vertices=num_polygon=0;
   worldv=camerav=NULL;
   polygons=NULL;
}


Polygon3d* Sector::IntersectSegment(const Vector3d& start,const Vector3d& end,
                                      Vector3d& isect, float* pr)
{
  for (int i = 0 ; i < num_polygon ; i++)
    {
      Polygon3d* p=(Polygon3d*)polygons[i];
//      if (p->IntersectSegment(start,end,isect,pr)) return p;
    }
  return NULL;
}

Sector* Sector::FollowSegment(ReversibleTransform& t,Vector3d& new_position)
{
  Vector3d isect;
  Polygon3d* p = /*sector->*/IntersectSegment (t.GetOrigin (), new_position, isect);
//  csPortal* po;

  if (p)
  {
//    po = p->GetPortal ();
//    if (po)
//      return po->FollowSegment (t, new_position, mirror);
//    else
      new_position = isect;
  }

  return this;
}



