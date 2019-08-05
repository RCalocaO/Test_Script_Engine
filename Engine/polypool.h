// 2d Polygon Pool
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __POLYPOOL_H__
#define __POLYPOOL_H__

#include "poly2d.h"

// Pool of Poly2d items For creating Poly2d objects or subclasses
// Basically it's a linked list of Poly2d
class Poly2DPool
{
private:
  struct PoolObj
  {
    PoolObj* next;
    Poly2d* pol2d;
  };
  // List of allocated polygons.
  PoolObj* alloced;
  // List of previously allocated, but now unused polygons.
  PoolObj* freed;

  // Factory for creating new polygons.
  Poly2DFactory* factory;

public:
  /// Create an empty pool.
  Poly2DPool (Poly2DFactory* fact) : alloced (NULL), freed (NULL),
  	factory (fact) { }

  /// Destroy pool and all objects in the pool.
  ~Poly2DPool ()
  {
    while (alloced)
    {
      PoolObj* n = alloced->next;
      Poly2d *tmp=alloced->pol2d;
      delete tmp;
      delete alloced;
      alloced = n;
    }
    while (freed)
    {
      PoolObj* n = freed->next;
      Poly2d *tmp=alloced->pol2d;
      delete tmp;
      delete freed;
      freed = n;
    }
  }


  /// Allocate a new object in the pool.
  Poly2d* Alloc ()
  {
    PoolObj* pnew;
    if (freed)
    {
      pnew = freed;
      freed = freed->next;
    }
    else
    {
      pnew = new PoolObj ();
      pnew->pol2d = factory->Create ();
//      pnew->pol2d = new Poly2d();
    }
    pnew->next = alloced;
    alloced = pnew;
    return pnew->pol2d;
  }

  // Free an object and put it back in the pool.
  void Free (Poly2d* pol)
  {
    if (alloced)
    {
      PoolObj* po = alloced;
      alloced = alloced->next;
      po->pol2d = pol;
      po->next = freed;
      freed = po;
    }
  }
};

#endif
