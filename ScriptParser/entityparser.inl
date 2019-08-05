// Current Entity being Parsed

static Entity *entity=NULL;

void ParseEntityPolygonVertex(Polygon3d *p)
{
  Match(T_VERTEX);
  Match(T_LP);
  double x=MatchNumber();
  Match(T_COMMA);
  double y=MatchNumber();
  Match(T_COMMA);
  double z=MatchNumber();
  Match(T_RP);
       // Add to polygon
       p->AddVertex((float)x, (float)y, (float)MatchNumber());
  Match(T_SEMICOLON);
}

int ParseEntityPolygonType(Polygon3d* p)
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
        // Set type of this Polygon
        p->SetType(type);
  Match(T_RP);
  Match(T_SEMICOLON);
  return type;
}

void ParseEntityPolygonVertexListElement(Polygon3d *p)
{
  double index=MatchNumber();
       // Add it to current polygon
       p->AddVertex((int)index);
  if (token==T_COMMA)
     {
        Match(T_COMMA);
        ParseEntityPolygonVertexListElement(p);
     }
}

void ParseEntityPolygonVertexList(Polygon3d *p)
{
  Match(T_VERTICES);
  Match(T_LP);
  ParseEntityPolygonVertexListElement(p);
  Match(T_RP);
  Match(T_SEMICOLON);
}


void ParseEntityPolygonInfo(Polygon3d *p)
{
   switch(token)
      {
        case T_VERTEX: ParseEntityPolygonVertex(p);
                       ParseEntityPolygonInfo(p);
                       break;
        case T_TYPE: ParseEntityPolygonType(p);
                     ParseEntityPolygonInfo(p);
                     break;
        case T_COLOR: Match(T_COLOR);
                      Match(T_LP);
                           // Color of polygon
                           p->SetColor((int)MatchNumber());
                      Match(T_RP);
                      Match(T_SEMICOLON);
                      ParseEntityPolygonInfo(p);
                      break;
        case T_VERTICES: ParseEntityPolygonVertexList(p);
                         ParseEntityPolygonInfo(p);
                         break;
      }
}

void ParseEntityPolygon()
{
  Match(T_POLYGON);
  //cout << "\t\tAdding Polygon3d: " << tokenstring << endl;
  Match(T_STRING);
  Match(T_LB);
       // New Polygon
       Polygon3d *p=entity->NewPolygon();
  ParseEntityPolygonInfo(p);
  Match(T_RB);
}

void ParseEntityCenter()
{
  Match(T_CENTER);
  Match(T_LP);
  double x=MatchNumber();
  Match(T_COMMA);
  double y=MatchNumber();
  Match(T_COMMA);
  double z=MatchNumber();
  Match(T_RP);
        // Set Entity Center
        entity->center=Vector3d(x,y,z);
  Match(T_SEMICOLON);
}

void ParseEntityVertex()
{
  Match(T_VERTEX);
  Match(T_LP);
  double x=MatchNumber();
  Match(T_COMMA);
  double y=MatchNumber();
  Match(T_COMMA);
  double z=MatchNumber();
        // Add this vertex to Entity Vertex List
        entity->AddVertex((float)x, (float)y, (float)z);
  Match(T_RP);
  Match(T_SEMICOLON);
}

void Parse_EntityStmt()
{
  if (token!=T_RB)
    {
      switch(token)
        {
          case T_POLYGON: ParseEntityPolygon();
                          Parse_EntityStmt();
                          break;
          case T_VERTEX: ParseEntityVertex();
                         Parse_EntityStmt();
                         break;
          case T_CENTER: ParseEntityCenter();
                         Parse_EntityStmt();
                         break;
        }
    }
}

void Parse_Entity()
{
  Match(T_ENTITY);
      // Create the New Entity
      entity=World::current_world->NewEntity(tokenstring);
       // Add it to Global Table
      type=T_ENTITY_VECTOR;
      // Search if constant is inserted into const identifiers sym table
      LocalVariableTable *where=SymTable->Search("GLOBAL")->local;
      LV_Entry *constnt=where->Search(tokenstring,false);
      // If it is not found, insert it
      if (constnt)
          SemanticError("<<<< ENTITY ALREADY DEFINED!!!!! >>>>");
      where->Insert(tokenstring,type);
      constnt=where->Search(tokenstring,false);
  Match(T_ID);
  Match(T_LB);
  Parse_EntityStmt();
  Match(T_RB);
}


