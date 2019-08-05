// 3d Polygon Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "polygon3d.h"
#include "polyset.h"
#include "sector.h"
#include "polygon2d.h"
#include "camera.h"
#include "../world.h"

#ifdef SMALL_Z
#undef SMALL_Z
#define SMALL_Z 1
#endif

Polygon3d::Polygon3d()
{
#if 0
  cout << "\t\tCreating Polygon..."<<endl;
#endif
  next=NULL;
  num_vertices=max_vertices=0;
  index=NULL;
//  plane = NULL;
}

Polygon3d::~Polygon3d()
{
#if 0
	cout << "\t\tDestroying Polygon..." << endl;
#endif
  if (index) delete [] index;
  num_vertices=max_vertices=0;
  index=NULL;
//  if (plane) delete plane;
}

int Polygon3d::AddVertex ( Vector3d& v)
{
  int i = poly_set->AddVertexSmart(v);
  AddVertex (i);
  return i;
}

int Polygon3d::AddVertex (float x, float y, float z)
{
  int i = poly_set->AddVertexSmart (x, y, z);
  AddVertex (i);
  return i;
}

int Polygon3d::AddVertex (int v)
{
  // Adds this entry to current index table
  if (!index)
    {
      index=new int[10];
      max_vertices=10;
    }
  while (num_vertices>=max_vertices)
    {
      max_vertices+=10;
      int *new_v=new int[max_vertices];
      memcpy(new_v,index,sizeof(int)*num_vertices);
      delete [] index;
      index=new_v;
    }
  index[num_vertices]=v;
  num_vertices++;
  return num_vertices-1;
}

/*
bool Polygon3d::DoPerspective( Camera& camera,Vector3d* source,
   	  int num_verts,Polygon2d* dest)
{
/*
  Vector3d *ind, *end = source+num_verts;
  if (num_verts==0) return false;
  dest->MakeEmpty ();

  // Classify all points as NORMAL (z>=SMALL_Z), NEAR (0<=z<SMALL_Z), or
  // BEHIND (z<0).  Use several processing algorithms: trivially accept if all
  // points are NORMAL, mixed process if some points are NORMAL and some
  // are not, special process if there are no NORMAL points, but some are
  // NEAR.  Assume that the polygon has already been culled if all points
  // are BEHIND.

  // Handle the trivial acceptance case:
  ind = source;
  while (ind < end)
  {
    if (ind->z >= SMALL_Z) dest->AddPerspective (*ind);
    else break;
    ind++;
  }

  // Check if special or mixed processing is required
  if (ind != end)
  {
    // If we are processing a triangle (uv_coords != NULL) then
    // we stop here because the triangle is only visible if all
    // vertices are visible (this is not exactly true but it is
    // easier this way! @@@ CHANGE IN FUTURE).

    Vector3d *exit = NULL, *exitn = NULL, *reenter = NULL, *reentern = NULL;
    Vector2d *evert = NULL;

    if (ind == source)
    {
      while (ind < end)
      {
        if (ind->z >= SMALL_Z) { reentern = ind;  reenter = ind-1;  break; }
        ind++;
      }
    }
    else
    {
      exit = ind;
      exitn = ind-1;
      evert = dest->GetLast ();
    }

    // Check if mixed processing is required
    if (exit || reenter)
    {
     bool needfinish = false;

     if (exit)
     {
      // we know where the polygon is no longer NORMAL, now we need to
      // to find out on which edge it becomes NORMAL again.
      while (ind < end)
      {
       if (ind->z >= SMALL_Z) { reentern = ind;  reenter = ind-1;  break; }
       ind++;
      }
      if (ind == end) { reentern = source;  reenter = ind-1; }
       else needfinish = true;
     }
     else
     {
      // we know where the polygon becomes NORMAL, now we need to
      // to find out on which edge it ceases to be NORMAL.
      while (ind < end)
      {
       if (ind->z >= SMALL_Z) dest->AddPerspective (*ind);
       else { exit = ind;  exitn = ind-1;  break; }
       ind++;
      }
      if (ind == end) { exit = source;  exitn = ind-1; }
      evert = dest->GetLast ();
     }

     // Add the NEAR points appropriately.
#    define MAX_VALUE 1000000.

     // First, for the exit point.
     float ex, ey, epointx, epointy;
     ex = exitn->z * exit->x - exitn->x * exit->z;
     ey = exitn->z * exit->y - exitn->y * exit->z;
     if (ABS(ex) < SMALL_EPSILON && ABS(ey) < SMALL_EPSILON)
     {
      // Uncommon special case:  polygon passes through origin.
      plane->WorldToCamera(camera, source[0]); //@@@ Why is this needed???
      ex = plane->GetCameraPlane ().A();
      ey = plane->GetCameraPlane ().B();
      if (ABS(ex) < SMALL_EPSILON && ABS(ey) < SMALL_EPSILON)
      {
       // Downright rare case:  polygon near parallel with viewscreen.
       ex = exit->x - exitn->x;
       ey = exit->y - exitn->y;
      }
     }
     if (ABS(ex) > ABS(ey))
     {
       if (ex>0) epointx = MAX_VALUE;
       else epointx = -MAX_VALUE;
       epointy = (epointx - evert->x)*ey/ex + evert->y;
     }
     else
     {
       if (ey>0) epointy = MAX_VALUE;
       else epointy = -MAX_VALUE;
       epointx = (epointy - evert->y)*ex/ey + evert->x;
     }

     // Next, for the reentry point.
     float rx, ry, rpointx, rpointy;

     // Perspective correct the point.
     float iz = Camera::aspect/reentern->z;
     Vector2d rvert;
     rvert.x = reentern->x * iz + World::shift_x;
     rvert.y = reentern->y * iz + World::shift_y;

     if (reenter == exit && reenter->z > -SMALL_EPSILON)
     { rx = ex;  ry = ey; }
     else
     {
       rx = reentern->z * reenter->x - reentern->x * reenter->z;
       ry = reentern->z * reenter->y - reentern->y * reenter->z;
     }
     if (ABS(rx) < SMALL_EPSILON && ABS(ry) < SMALL_EPSILON)
     {
      // Uncommon special case:  polygon passes through origin.
      plane->WorldToCamera(camera,source[0]); //@@@ Why is this needed?
      rx = plane->GetCameraPlane ().A();
      ry = plane->GetCameraPlane ().B();
      if (ABS(rx) < SMALL_EPSILON && ABS(ry) < SMALL_EPSILON)
      {
       // Downright rare case:  polygon near parallel with viewscreen.
       rx = reenter->x - reentern->x;
       ry = reenter->y - reentern->y;
      }
     }
     if (ABS(rx) > ABS(ry))
     {
       if (rx>0) rpointx = MAX_VALUE;
       else rpointx = -MAX_VALUE;
       rpointy = (rpointx - rvert.x)*ry/rx + rvert.y;
     }
     else
     {
       if (ry>0) rpointy = MAX_VALUE;
       else rpointy = -MAX_VALUE;
       rpointx = (rpointy - rvert.y)*rx/ry + rvert.x;
     }

#    define QUADRANT(x,y) ( (y<x?1:0)^(x<-y?3:0) )
#    define MQUADRANT(x,y) ( (y<x?3:0)^(x<-y?1:0) )

    dest->AddVertex (epointx,epointy);
#   if EXPERIMENTAL_BUG_FIX
    if (mirror)
    {
      int quad = MQUADRANT(epointx, epointy);
      int rquad = MQUADRANT(rpointx, rpointy);
      if ((quad==0 && -epointx==epointy)||(quad==1 && epointx==epointy))
        quad++;
      if ((rquad==0 && -rpointx==rpointy)||(rquad==1 && rpointx==rpointy))
        rquad++;
      while (quad != rquad)
      {
        epointx = (quad&2)           ?  MAX_VALUE : -MAX_VALUE;
        epointy = (quad==0||quad==3) ?  MAX_VALUE : -MAX_VALUE;
        dest->AddVertex (epointx, epointy);
        quad = (quad+1)&3;
      }
    }
    else
    {
      int quad = QUADRANT(epointx, epointy);
      int rquad = QUADRANT(rpointx, rpointy);
      if ((quad==0 && epointx==epointy)||(quad==1 && -epointx==epointy))
        quad++;
      if ((rquad==0 && rpointx==rpointy)||(rquad==1 && -rpointx==rpointy))
        rquad++;
      while (quad != rquad)
      {
        epointx = (quad&2)           ? -MAX_VALUE :  MAX_VALUE;
        epointy = (quad==0||quad==3) ?  MAX_VALUE : -MAX_VALUE;
        dest->AddVertex (epointx, epointy);
        quad = (quad+1)&3;
      }
    }
#   endif
    dest->AddVertex (rpointx,rpointy);

     // Add the rest of the vertices, which are all NORMAL points.
     if (needfinish) while (ind < end)
      dest->AddPerspective (*ind++);

    }

    // Do special processing (all points are NEAR or BEHIND)
    else
    {
        Vector3d* ind2 = end - 1;
        for (ind = source;  ind < end;  ind2=ind, ind++)
          if ((ind->x - ind2->x)*(ind2->y) - (ind->y - ind2->y)*(ind2->x) < SMALL_EPSILON)
            return false;
        dest->AddVertex (-MAX_VALUE,-MAX_VALUE);
        dest->AddVertex (-MAX_VALUE, MAX_VALUE);
        dest->AddVertex ( MAX_VALUE, MAX_VALUE);
        dest->AddVertex ( MAX_VALUE,-MAX_VALUE);
    }
  }
  return true;
*/


bool Polygon3d::DoPerspective( Camera& camera,Vector3d* source,
   	  int num_verts,Polygon2d* dest)
{
  if (num_verts==0) return false;
  dest->MakeEmpty ();
  Vector3d *ind, *end = source+num_verts;
  ind = source;
  while (ind < end)
  {
    dest->AddPerspective (*ind);
    ind++;
  }

  return true;
}


bool Polygon3d::ZClip(Vector3d* finalverts,int& num_verts,bool cw)
{
  int i, cnt_vis;
  static Vector3d verts[100];
  // If there are no visible vertices this polygon need not be drawn
  cnt_vis = 0;
  for (i = 0 ; i<num_vertices; i++)
    if ( (verts[i]=poly_set->camerav[index[i]]).z >= SMALL_Z) cnt_vis++;
  if (cnt_vis == 0)
    return false;
  int numv=num_vertices;
  // Clip this polygon in 3D against the Z Plane
  // Sutherland Codgeman Z Clip!
  int start=numv-1,end;
  int count=0;
  for (end=0; end<numv; end++)
   {
      if (verts[end].z>SMALL_Z)
        {
          if (verts[start].z>SMALL_Z)
            {
              finalverts[count].x=verts[end].x;
              finalverts[count].y=verts[end].y;
              finalverts[count].z=verts[end].z;
              count++;
            }
           else
            {
              // Add cross & end?
              double t=(SMALL_Z-verts[start].z)/(verts[end].z-verts[start].z);
              finalverts[count].x=verts[start].x+t*(verts[end].x-verts[start].x);
              finalverts[count].y=verts[start].y+t*(verts[end].y-verts[start].y);
              finalverts[count].z=SMALL_Z;
              count++;
              finalverts[count].x=verts[end].x;
              finalverts[count].y=verts[end].y;
              finalverts[count].z=verts[end].z;
              count++;
            }
        }
       else
        {
          if (verts[start].z>SMALL_Z)
            {
              // Add cross but not end?
              double t=(SMALL_Z-verts[start].z)/(verts[end].z-verts[start].z);
              finalverts[count].x=verts[start].x+t*(verts[end].x-verts[start].x);
              finalverts[count].y=verts[start].y+t*(verts[end].y-verts[start].y);
              finalverts[count].z=SMALL_Z;
              count++;
            }
           else
            {
              // Nothing!
            }
        }
      start=end;
   }
  num_verts=count;
  return true;
}


