// Poly2d Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "poly2d.h"
#include "clipper.h"

Poly2DFactory* Poly2DFactory::SharedFactory()
{
  static Poly2DFactory* p = 0;
  if (p == 0)
    p = new Poly2DFactory;
  return p;
}


Poly2d::Poly2d (int start_size)
{
  max_vertices = start_size;
  vertices = new Vector2d[max_vertices];
  MakeEmpty ();
}

Poly2d::Poly2d (Poly2d& copy)
{
  max_vertices = copy.max_vertices;
  vertices = new Vector2d[max_vertices];
  num_vertices = copy.num_vertices;
  memcpy (vertices, copy.vertices, sizeof (Vector2d)*num_vertices);
  bbox = copy.bbox;
}

Poly2d::~Poly2d ()
{
  delete [] vertices;
}

void Poly2d::MakeEmpty ()
{
  num_vertices = 0;
  bbox.StartBoundingBox ();
}

void Poly2d::MakeRoom (int new_max)
{
  if (new_max <= max_vertices) return;
  Vector2d* new_vertices = new Vector2d[new_max];
  memcpy (new_vertices, vertices, num_vertices*sizeof(Vector2d));
  delete [] vertices;
  vertices = new_vertices;
  max_vertices = new_max;
}

void Poly2d::AddVertex (double x,double y)
{
  if (num_vertices >= max_vertices)
    MakeRoom (max_vertices+5);
  vertices[num_vertices].x = x;
  vertices[num_vertices].y = y;
  num_vertices++;
  bbox.AddBoundingVertex ((float)x, (float)y);
}

void Poly2d::Draw(int color)
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
    line(video,x1,y1,x2,y2, color);
#endif
    x1 = x2;
    y1 = y2;
  }
}

bool Poly2d::ClipAgainst (const Clipper* view)
{
  MakeRoom (num_vertices+view->GetNumVertices ()+1);
  return view->Clip (vertices, num_vertices, max_vertices, &bbox);
}

/*
void Poly2d::MakeEmpty ()
{
  num_vertices = 0;
  bbox.StartBoundingBox ();
}
*/

