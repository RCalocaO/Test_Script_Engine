// Polygon2d Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "polygon2d.h"
#include "sector.h"
#include "../world.h"
#include "camera.h"
#include "../trohs3d.h"

Polygon2DFactory* Polygon2DFactory::SharedFactory()
{
  static Polygon2DFactory* p = 0;
  if (p == 0)
    p=new Polygon2DFactory;
  return p;
}

void Polygon2d::AddPerspective(double x,double y,double z)
{
  if (num_vertices >= max_vertices)
    MakeRoom (max_vertices+5);

  float iz = Camera::aspect/z;
  float px, py;

  px = x * iz + World::shift_x;
  py = y * iz + World::shift_y;
  vertices[num_vertices].x = px;
  vertices[num_vertices].y = py;

  num_vertices++;
  bbox.AddBoundingVertex (px, py);
}

// WireFrame
void Polygon2d::DrawWireframe(int col) const
{
  int i;
  int x1, y1, x2, y2;
  if (!num_vertices) return;
  x1 = QRound (vertices[num_vertices-1].x);
  y1 = QRound (vertices[num_vertices-1].y);
  for (i = 0 ; i < num_vertices ; i++)
  {
    x2 = QRound (vertices[i].x);
    y2 = QRound (vertices[i].y);
#if 0
    line (video,x1,World::frame_height-1- y1,x2,World::frame_height-1-y2,col);
#endif
    x1 = x2;
    y1 = y2;
  }
}

// Vertices
void Polygon2d::Draw(int col) const
{
#if 0
	int i;
	for (i = 0; i < num_vertices; i++)
      putpixel(video,QRound(vertices[i].x),QRound(vertices[i].y),col);
#endif
}

// Solid Filled Color!
void Polygon2d::DrawSolid(int col) const
{
  if (!num_vertices) return;
  int *xy=new int[num_vertices*2];
  for (int i = 0 ; i < num_vertices ; i++)
  {
    xy[i*2]=QRound(vertices[i].x);
    xy[i*2+1]=QRound(vertices[i].y);
  }
#if 0
  polygon(video,num_vertices,xy,col);
#endif
  delete [] xy;
}

// Texture Mapped Polygon
void Polygon2d::DrawTexture() const
{
  if (!num_vertices) return;
//  V3D_f *v=new V3D_f[num_vertices];
/*  for (int i = 0 ; i < num_vertices ; i++)
  {
    v[i].x=vertices[i].x;
    v[i].y=vertices[i].y;
    v[i].z=vertices[i].z;
//    v[i].u=;
//    v[i].v=0;
  }
  polygon3D_f(video,POLYTYPE_PTEX,texture,num_vertices,v);
  delete [] v;*/
}


