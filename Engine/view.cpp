// View Camera & Clipper
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#include "view.h"
#include "clipper.h"
#include "camera.h"
#include "../world.h"
#include "polygon2d.h"

View::View (World *iWorld)
{
  world = iWorld;
  view = new Polygon2d();
  camera = new Camera();
  clipper = NULL;
}

View::~View ()
{
  delete camera;
  delete view;
  delete clipper;
}

void View::ClearView ()
{
  if (view) view->MakeEmpty ();
}

void View::SetRectangle (int x, int y, int w, int h)
{
  delete view;  view = NULL;
  bview = BoundBox((float)x, (float)y, x+w-1.0f, y+h-1.0f);
  delete clipper; clipper = NULL;
}

void View::AddViewVertex (int x, int y)
{
  if (!view) view = new Polygon2d();
  view->AddVertex (x, y);
  delete clipper; clipper = NULL;
}

void View::Draw ()
{
  if (!clipper)
    if (view) clipper = new PolygonClipper(view);
     else
         clipper = new BoxClipper(bview);
  world->Draw(camera,clipper);
}

void View::SetSector (Sector *sector)
{
  camera->SetSector (sector);
}

