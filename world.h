// World Class for Trohs 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#ifndef __WORLD_H__
#define __WORLD_H__

#include "trohs3d.h"
class Sector;
class Entity;
class Clipper;
class Camera;
class Poly2DPool;

class World
{
  public:
    World();
    ~World();
    Sector* NewSector();
    Entity* NewEntity(char *name);

    Entity* GetEntity(char *name);

    // Draw all the World using camera and clipper object
    void Draw( Camera *cam, Clipper *clipper);

    // Prepare World
    bool Initialize();

    // An object pool for 2D polygons used by the rendering process.
    Poly2DPool* render_pol2d_pool;

    // Shift for perspective correction (corresponds to half of screen size).
    static double shift_x, shift_y;

    // Remember dimensions of display.
    static int frame_width, frame_height;

    // Current world.
    static World* current_world;

  protected:
    int sector_count;
    Sector* sectors;
    void DeleteSectors(Sector *head);

    int entity_count;
    Entity* entities;
    void DeleteEntities(Entity *head);
};

#endif