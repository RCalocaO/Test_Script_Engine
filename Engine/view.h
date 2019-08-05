// View Camera & Clipper
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __VIEW_H__
#define __VIEW_H__

#include "../trohs3d.h"
#include "vector2d.h"
#include "boundbox.h"

class Camera;
class Clipper;
class World;
class Sector;
class Polygon2d;
class BoundBox;

class View
{
  public:
    World *world;
    Camera *camera;
    Clipper* clipper;
    View(World *iWorld);
    ~View();
    World* GetWorld () { return world; }

    // Clipping rectangle.
    BoundBox bview;

    // Clipping region.
    Polygon2d *view;

    // Clear clipping polygon.
    void ClearView ();

    // Get current camera.
    Camera* GetCamera () { return camera; }

    // Set current camera.
    void SetCamera (Camera* c) { camera = c; }

    // Set clipping rectangle.
    void SetRectangle (int x, int y, int w, int h);

    // Add a vertex to clipping polygon (non-rectangular clipping).
    void AddViewVertex (int x, int y);

    // Draw world as seen from the camera.
    void Draw();

    // Set sector for the current camera.
    void SetSector(Sector *sector);

    // Return the clipper.
    Clipper* GetClipper () { return clipper; }
};

#endif
