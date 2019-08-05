// Clipper
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "clipper.h"

Poly2DPool Clipper::polypool(Poly2DFactory::SharedFactory());

bool BoxClipper::Clip (Vector2d *Polygon, Vector2d* dest_poly, int Count,
	int &OutCount) const
{
  memcpy (dest_poly, Polygon, Count * sizeof (Vector2d));
  OutCount = Count;
  BoundBox bbox = BoundBox(-100000., -100000., 100000., 100000.);
  if (Clip(dest_poly, OutCount, Count+5, &bbox)) return true;
   else
   {
    OutCount = 0;
    return false;
   }
}

bool BoxClipper::Clip (Vector2d *Polygon, int& Count, int MaxCount,
                       BoundBox *BoundingBox) const
{
  Vector2d OutPoly[100];
  Vector2d *InPolygon = Polygon;
  int InVertices = Count;
  Vector2d *OutPolygon = OutPoly;
  int OutVertices = Count;
  int vert; 

  if (!region.Overlap (*BoundingBox)) return false;

  
  if (BoundingBox->MinX () < region.MinX ()) // clip against left x line
  {
    float px = (float)InPolygon[0].x, py = (float)InPolygon[0].y;
    bool pVertIn = (px > region.MinX ());
    OutVertices = 0;
    int isectCount = 0;
    for (vert = 1; vert <= InVertices; vert++)
    {
     float cx, cy;
     if (vert < InVertices) 
     { cx = (float)InPolygon[vert].x;  cy = (float)InPolygon[vert].y; }
     else { cx = (float)InPolygon[0].x;  cy = (float)InPolygon[0].y; }
     bool cVertIn = (cx > region.MinX ());
     
     if (pVertIn)
     {
      OutPolygon[OutVertices].x = px;  OutPolygon[OutVertices].y = py;
      OutVertices++;
      if (OutVertices == MaxCount) break;
     }   
     if (pVertIn != cVertIn)
     {
      OutPolygon[OutVertices].x = region.MinX ();
      OutPolygon[OutVertices].y = cy - (cx-region.MinX ())*(cy-py)/(cx-px);
      OutVertices++;  
      if (OutVertices == MaxCount) break;
      isectCount++;
      if (isectCount >= 2)
      {
       if (cVertIn)
       {
        memcpy (&OutPolygon[OutVertices], &InPolygon[vert],
                (InVertices - vert) * sizeof(OutPolygon[0]));
        OutVertices += InVertices - vert;
       }
       break;
      }
     }
     px = cx; py = cy;  pVertIn = cVertIn; 
    } // for (vert = 1; vert <= InVertices; vert++)

    if (OutVertices < 3) { Count = 0; return false; }
    InVertices = OutVertices;
    InPolygon = OutPolygon;
    if (OutPolygon == OutPoly) OutPolygon = Polygon;
     else OutPolygon = OutPoly;
  }
  if (BoundingBox->MinY () < region.MinY ()) // clip against upper y line
  {
    float px = (float)InPolygon[0].x, py = (float)InPolygon[0].y;
    bool pVertIn = (py > region.MinY ());
    OutVertices = 0;
    int isectCount = 0;
    for (vert = 1; vert <= InVertices; vert++)
    {
     float cx, cy;
     if (vert < InVertices) 
     { cx = (float)InPolygon[vert].x;  cy = (float)InPolygon[vert].y; }
     else { cx = (float)InPolygon[0].x;  cy = (float)InPolygon[0].y; }
     bool cVertIn = (cy > region.MinY ());
     
     if (pVertIn)
     {
      OutPolygon[OutVertices].x = px;  OutPolygon[OutVertices].y = py;
      OutVertices++;
      if (OutVertices == MaxCount) break;
     }   
     if (pVertIn != cVertIn)
     {
      OutPolygon[OutVertices].x = cx - (cy-region.MinY ())*(cx-px)/(cy-py);
      OutPolygon[OutVertices].y = region.MinY ();
      OutVertices++;  
      if (OutVertices == MaxCount) break;
      isectCount++;
      if (isectCount >= 2)
      {
       if (cVertIn)
       {
        memcpy (&OutPolygon[OutVertices], &InPolygon[vert],
                (InVertices - vert) * sizeof(OutPolygon[0]));
        OutVertices += InVertices - vert;
       }
       break;
      }
     }
     px = cx; py = cy;  pVertIn = cVertIn; 
    } // for (vert = 1; vert <= InVertices; vert++)

    if (OutVertices < 3) { Count = 0; return false; }
    InVertices = OutVertices;
    InPolygon = OutPolygon;
    if (OutPolygon == OutPoly) OutPolygon = Polygon;
     else OutPolygon = OutPoly;
  }
  if (BoundingBox->MaxX () > region.MaxX ()) // clip against right x line
  {
    float px = (float)InPolygon[0].x, py = (float)InPolygon[0].y;
    bool pVertIn = (px < region.MaxX ());
    OutVertices = 0;
    int isectCount = 0;
    for (vert = 1; vert <= InVertices; vert++)
    {
     float cx, cy;
     if (vert < InVertices) 
     { cx = (float)InPolygon[vert].x;  cy = (float)InPolygon[vert].y; }
     else { cx = (float)InPolygon[0].x;  cy = (float)InPolygon[0].y; }
     bool cVertIn = (cx < region.MaxX ());
     
     if (pVertIn)
     {
      OutPolygon[OutVertices].x = px;  OutPolygon[OutVertices].y = py;
      OutVertices++;
      if (OutVertices == MaxCount) break;
     }   
     if (pVertIn != cVertIn)
     {
      OutPolygon[OutVertices].x = region.MaxX ();
      OutPolygon[OutVertices].y = cy - (cx-region.MaxX ())*(cy-py)/(cx-px);
      OutVertices++;  
      if (OutVertices == MaxCount) break;
      isectCount++;
      if (isectCount >= 2)
      {
       if (cVertIn)
       {
        memcpy (&OutPolygon[OutVertices], &InPolygon[vert],
                (InVertices - vert) * sizeof(OutPolygon[0]));
        OutVertices += InVertices - vert;
       }
       break;
      }
     }
     px = cx; py = cy;  pVertIn = cVertIn; 
    }

    if (OutVertices < 3) { Count = 0; return false; }
    InVertices = OutVertices;
    InPolygon = OutPolygon;
    if (OutPolygon == OutPoly) OutPolygon = Polygon;
     else OutPolygon = OutPoly;
  }
  if (BoundingBox->MaxY () > region.MaxY ()) // clip against lower y line
  {
    float px = (float)InPolygon[0].x, py = (float)InPolygon[0].y;
    bool pVertIn = (py < region.MaxY ());
    OutVertices = 0;
    int isectCount = 0;
    for (vert = 1; vert <= InVertices; vert++)
    {
     float cx, cy;
     if (vert < InVertices) 
     { cx = (float)InPolygon[vert].x;  cy = (float)InPolygon[vert].y; }
     else { cx = (float)InPolygon[0].x;  cy = (float)InPolygon[0].y; }
     bool cVertIn = (cy < region.MaxY ());
     
     if (pVertIn)
     {
      OutPolygon[OutVertices].x = px;  OutPolygon[OutVertices].y = py;
      OutVertices++;
      if (OutVertices == MaxCount) break;
     }   
     if (pVertIn != cVertIn)
     {
      OutPolygon[OutVertices].x = cx - (cy-region.MaxY ())*(cx-px)/(cy-py);
      OutPolygon[OutVertices].y = region.MaxY ();
      OutVertices++;  
      if (OutVertices == MaxCount) break;
      isectCount++;
      if (isectCount >= 2)
      {
       if (cVertIn)
       {
        memcpy (&OutPolygon[OutVertices], &InPolygon[vert],
                (InVertices - vert) * sizeof(OutPolygon[0]));
        OutVertices += InVertices - vert;
       }
       break;
      }
     }
     px = cx; py = cy;  pVertIn = cVertIn; 
    }

    if (OutVertices < 3) { Count = 0; return false; }
    InVertices = OutVertices;
    InPolygon = OutPolygon;
    if (OutPolygon == OutPoly) OutPolygon = Polygon;
     else OutPolygon = OutPoly;
  }

  Count = OutVertices;
  if (Count < 3) {  Count=0; return false; }
  if (InPolygon != Polygon) 
    memcpy (Polygon, InPolygon, Count * sizeof (Vector2d));
  BoundingBox->StartBoundingBox (*InPolygon++);
  while (--OutVertices)
    BoundingBox->AddBoundingVertexSmart (*InPolygon++);
  return true;
}

// ------------------------------------------
PolygonClipper::PolygonClipper(Poly2d *clipper,bool copy)
{
  int vert;
  int Count = clipper->GetNumVertices ();

  if (copy)
  {
    ClipPoly2D = polypool.Alloc ();
    ClipPoly2D->MakeRoom (Count);
    ClipPoly = ClipPoly2D->GetVertices ();
    for (vert = 0; vert < Count; vert++)
      ClipPoly [vert] = (*clipper) [vert];
  }
  else
  {
    ClipPoly2D = NULL;
    ClipPoly = clipper->GetVertices ();
  }

  ClipData = new SegData [ClipPolyVertices = Count];
  // Precompute some data for each clipping edge
  for (vert = 0; vert < ClipPolyVertices; vert++)
  {
    int next = (vert == ClipPolyVertices - 1 ? 0 : vert + 1);
    ClipData [vert].dx = ClipPoly [next].x - ClipPoly [vert].x;
    ClipData [vert].dy = ClipPoly [next].y - ClipPoly [vert].y;
  }

  // Initialize bounding box
  ClipBox.StartBoundingBox (ClipPoly [0]);
  for (vert = 1; vert < ClipPolyVertices; vert++)
    ClipBox.AddBoundingVertex (ClipPoly [vert]);
}

PolygonClipper::~PolygonClipper ()
{
  if (ClipData)
    delete [] ClipData;
  if (ClipPoly2D)
    polypool.Free(ClipPoly2D);
}

bool PolygonClipper::IsInside (double x, double y) const
{
  for (int vert = 0; vert < ClipPolyVertices; vert++)
    if (x * ClipData [vert].dy - y * ClipData [vert].dx < 0)
      return false;
  return true;
}

bool PolygonClipper::Clip(Vector2d *Polygon,Vector2d* dest_poly, int Count, int &OutCount) const
{
#  include "polyclip.inl"

  if (OutVertices < 3)
  {
    OutCount = 0;
    return false;
  } else
  {
    memcpy (dest_poly, InPolygon, OutVertices * sizeof (Vector2d));
    OutCount = OutVertices;
    return true;
  }
}

bool PolygonClipper::Clip(Vector2d *Polygon, int &Count, int MaxCount,
  BoundBox *BoundingBox) const
{
  if (!ClipBox.Overlap (*BoundingBox))
    return false;

#  include "polyclip.inl"

  if (OutVertices < 3)
  {
    Count = 0;
    return false;
  } else
  {
    if (OutVertices > MaxCount)
      OutVertices = MaxCount;
    Count = OutVertices;
    memcpy (Polygon, InPolygon, Count *sizeof(Vector2d));
    BoundingBox->StartBoundingBox (*InPolygon++);
    while (--OutVertices)
      BoundingBox->AddBoundingVertexSmart (*InPolygon++);
    return true;
  }
}

