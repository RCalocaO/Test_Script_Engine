// World Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "world.h"
#include "engine/sector.h"
#include "entity.h"
#include "engine/clipper.h"
#include "engine/camera.h"
#include "engine/polypool.h"
#include "engine/poly2d.h"
#include "engine/polygon2d.h"
#include <conio.h>

World* World::current_world = NULL;
double World::shift_x;
double World::shift_y;
int World::frame_width;
int World::frame_height;

World::World()
{
#if 0
   cout << "Creating World..." << endl;
#endif
   // No Sectors at the beginning
   sectors=NULL;
   sector_count=0;
   entities=NULL;
   entity_count=0;
   render_pol2d_pool = new Poly2DPool (Polygon2DFactory::SharedFactory());
}

World::~World()
{
#if 0
	clrscr();
   cout << "Destroying World..." << endl;
#endif
   // Delete all Sectors
   DeleteSectors(sectors);
   sector_count=0;
   DeleteEntities(entities);
   entity_count=0;
//   delete render_pol2d_pool;
}

Sector* World::NewSector()
{
   // Create in a temporary variable
   Sector *ns=new Sector();
   // Add to internal list
   if (sectors==NULL)
     sectors=ns;
    else
      {
         Sector *l=sectors;
         while (l->next!=NULL)
           l=(Sector*)l->next;
         l->next=ns;
      }
   ++sector_count;
   // Return a pointer to it
   return ns;
}

Entity* World::NewEntity(char *name)
{
   // Create in a temporary variable
   Entity *ns=new Entity(name);
   // Add to internal list
   if (entities==NULL)
     entities=ns;
    else
      {
         Entity *l=entities;
         while (l->next!=NULL)
           l=(Entity*)l->next;
         l->next=ns;
      }
   ++entity_count;
   // Return a pointer to it
   return ns;
}

// Recursive Delete of Linked List of Sectors
void World::DeleteSectors(Sector *head)
{
   if (head==NULL)
     return;
   if (head->next!=NULL)
     DeleteSectors((Sector*)head->next);
   delete head;
   head=NULL;
}

// Recursive Delete of Linked List of Sectors
void World::DeleteEntities(Entity *head)
{
   if (head==NULL)
     return;
   if (head->next!=NULL)
     DeleteEntities((Entity*)head->next);
   delete head;
   head=NULL;
}

void World::Draw( Camera *cam, Clipper *clipper)
{
  // Get Sector from Camera and call Draw
  cam->GetSector()->Draw(cam,clipper);
  // Draw Entities
  Entity *list=entities;
  while (list!=NULL)
    {
       list->Draw(cam,clipper);
       list=(Entity*)list->next;
    }
}

bool World::Initialize (/*ISystem* sys*/)
{
#if 0
  frame_width=SCREEN_W;
  frame_height=SCREEN_H;
#endif
  shift_x = (double)(frame_width/2);
  shift_y = (double)(frame_height/2);
  current_world = this;
  if (Camera::aspect == 0) Camera::aspect = frame_height;
  Camera::inv_aspect = 1.0/Camera::aspect;
  return true;
}

Entity* World::GetEntity(char *name)
{
   Entity *e=entities;
   while (e!=NULL)
     {
        if (strcmp(e->id,name)==0)
          return e;
        e=(Entity*)e->next;
     }
   return NULL;
}

