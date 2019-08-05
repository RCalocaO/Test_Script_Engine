// World Parser Class for 3D Engine
// Parses info for A World Definition Class
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "../Scanner/script.h"
#include "worldparser.h"
#include "../trohs3d.h"
#include "../world.h"
#include "../engine/sector.h"
#include "../engine/polygon3d.h"
#include "../engine/view.h"
#include "../engine/camera.h"
#include <stdio.h>

// Fwd Declarations
bool WorldParse(World *w,View *v);
void ParseSectorInfo(Sector *s,View *v);
void ParseWorldInfo(World *w,View *v);

// Main Function
bool LoadWorldFromFile(World *w,View *v,const char *filename)
{
#if 0
  cout << "Parsing World File..." << endl;
#endif
  // Try to open file
  FILE *f=fopen(filename,"r");
  if (!f) return false;
  script_in=f;
  bool result=WorldParse(w,v);
  fclose(f);
  return result;
}


// Useful Functions
static TokenType token;
static TokenType type;
static bool flag=true;

static void Match(TokenType expected)
{
   if (token==expected)
     token=GetToken();
    else
     {
       SyntaxError("Unexpected Token: ");
       PrintToken(token,tokenstring);
       fprintf(script_list,"          ");
       flag=false;
     }
}

// Returns the Number
static double MatchNumber()
{
  double value;
  bool minus=false;
  if (token==T_MINUS)
    {
      Match(T_MINUS);
      minus=true;
    }
  if (token==T_NUM_INT)
    {
      value=atof(tokenstring);
      Match(T_NUM_INT);
    }
   else if (token==T_NUM_REAL)
    {
      value=atof(tokenstring);
      Match(T_NUM_REAL);
    }
  if (minus)
    value=-value;
  return value;
}

// Returns Poly type, Vertices, SOlid, Wireframe or Textured
int ParsePolygon3dType(Polygon3d *p)
{
  int type=-1;
  Match(T_TYPE);
  Match(T_LP);
  switch(token)
    {
       case T_VERTICES: Match(T_VERTICES);
                        type=DRAW_VERTICES;
                        break;
       case T_SOLID: Match(T_SOLID);
                     type=DRAW_SOLID;
                     break;
       case T_WIREFRAME: Match(T_WIREFRAME);
                         type=DRAW_WIREFRAME;
                         break;
       case T_TEXTURE: Match(T_TEXTURE);
                       type=DRAW_TEXTUREMAP;
                       break;
    }
  Match(T_RP);
  Match(T_SEMICOLON);
  p->SetType(type);
  return type;
}

// A Vertex is VERTEX(0,0,0);
void ParsePolygon3dVertex(Polygon3d *p)
{
  Match(T_VERTEX);
  Match(T_LP);
  double x=MatchNumber();
  Match(T_COMMA);
  double y=MatchNumber();
  Match(T_COMMA);
  p->AddVertex((float)x, (float)y, (float)MatchNumber());
  Match(T_RP);
  Match(T_SEMICOLON);
}

// Load indices
void ParsePolygon3dVertexListElement(Sector *s,Polygon3d *p)
{
  double index=MatchNumber();
  p->AddVertex((int)index);
  if (token==T_COMMA)
     {
        Match(T_COMMA);
        ParsePolygon3dVertexListElement(s,p);
     }
}

void ParsePolygon3dVertexList(Sector *s,Polygon3d *p)
{
  Match(T_VERTICES);
  Match(T_LP);
  ParsePolygon3dVertexListElement(s,p);
  Match(T_RP);
  Match(T_SEMICOLON);
}

// Members of a Polygon3d
void ParsePolygon3dInfo(Sector* s,Polygon3d *p)
{
   switch(token)
      {
        case T_VERTEX: ParsePolygon3dVertex(p);
                         ParsePolygon3dInfo(s,p);
                         break;
        case T_TYPE: ParsePolygon3dType(p);
                     ParsePolygon3dInfo(s,p);
                     break;
        case T_COLOR: Match(T_COLOR);
                      Match(T_LP);
                      p->SetColor((int)MatchNumber());
                      Match(T_RP);
                      Match(T_SEMICOLON);
                      ParsePolygon3dInfo(s,p);
                      break;
        case T_VERTICES: ParsePolygon3dVertexList(s,p);
                         ParsePolygon3dInfo(s,p);
                         break;
      }
}

// One Polygon3d or a Camera Definition
void ParsePolygon3d(Sector *s,View* v)
{
  Match(T_POLYGON);
  //cout << "\t\tAdding Polygon3d: " << tokenstring << endl;
  Match(T_STRING);
  Match(T_LB);
  Polygon3d *p=s->NewPolygon();
  ParsePolygon3dInfo(s,p);
  Match(T_RB);
  ParseSectorInfo(s,v);
}

// Set Camera Position and Sector
void ParseCamera(Sector *s,View *v)
{
  Match(T_CAMERA);
  // Assign Camera to this Sector
  v->SetSector(s);
  // Now Load Start Position. If no given, assume @ origin(0,0,0)
  if (token==T_SEMICOLON)
    v->GetCamera()->SetPosition(Vector3d(0,0,0));
   else
    {
      // A Vector (x,y,z)
      Match(T_LP);
      double x=MatchNumber();
      Match(T_COMMA);
      double y=MatchNumber();
      Match(T_COMMA);
      v->GetCamera()->SetPosition(Vector3d(x,y,MatchNumber()));
      Match(T_RP);
    }
  Match(T_SEMICOLON);
}

// Add a Vertex to the Sector
void ParseSectorVertex(Sector *s)
{
  Match(T_VERTEX);
  Match(T_LP);
  double x=MatchNumber();
  Match(T_COMMA);
  double y=MatchNumber();
  Match(T_COMMA);
  s->AddVertex((float)x, (float)y, (float)MatchNumber());
  Match(T_RP);
  Match(T_SEMICOLON);
}


// Vertex List, Polygon List or Camera
void ParseSectorInfo(Sector *s,View *v)
{
  if (token!=T_RB)
    switch(token)
      {

         case T_POLYGON: ParsePolygon3d(s,v);
                         ParseSectorInfo(s,v);
                         break;
         case T_CAMERA: ParseCamera(s,v);
                        ParseSectorInfo(s,v);
                        break;
         case T_VERTEX: ParseSectorVertex(s);
                        ParseSectorInfo(s,v);
                        break;
      }
}

// One Sector! Is composed of Polygons, Camera or Vertices!
void ParseSector(World *w,View *v)
{
  Match(T_SECTOR);
#if 0
  cout << "\tCreating Sector: " << tokenstring << endl;
#endif
  Match(T_STRING);
  Match(T_LB);
  Sector *s=w->NewSector();
  ParseSectorInfo(s,v);
  Match(T_RB);
  ParseWorldInfo(w,v);
}


// Parse various Sectors
void ParseWorldInfo(World *w,View *v)
{
  switch(token)
    {
       case T_SECTOR: ParseSector(w,v);
                      break;
    }
}

// Main Parsing Function. Begins by extracting tokens from World File
bool WorldParse(World *w,View *v)
{
  token=GetToken();
  Match(T_WORLD);
#if 0
  cout << "Creating World: " << tokenstring << endl;
#endif
  Match(T_STRING);
  Match(T_LB);
  ParseWorldInfo(w,v);
  Match(T_RB);
  return flag;
}



