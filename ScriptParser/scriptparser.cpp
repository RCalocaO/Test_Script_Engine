// Script Parser for Trohs 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "scriptparser.h"
#include "../Scanner/script.h"
#include "../trohs3d.h"
#include "../world.h"
#include "../engine/sector.h"
#include "../engine/polyset.h"
#include "../entity.h"
#include "../engine/polygon3d.h"
#include "icode.h"
#include "proc_dir.h"
#include "quad.h"
#include "stacks.h"
#include "address.h"
#include <stdio.h>

void BeginParse();

QuadList* ParseScript(const char *scriptfilename)
{
#if 0
  cout << "Parsing Script File..." << endl;
#endif
  FILE *f=fopen(scriptfilename,"r");
  if (!f) return NULL;
  script_in=f;
  lineno=0;
  script_list=stdout;
  BeginParse();
  return ICode;
}


static TokenType token;
static TokenType type;

// Useful Functions
static void Match(TokenType expected)
{
   if (token==expected)
     token=GetToken();
    else
     {
       SyntaxError("Unexpected Token: ");
       PrintToken(token,tokenstring);
       fprintf(script_list,"          ");
     }
}


// Matches a Number, gets next token and returns the Number
// Used when the direct value is needed, not necessary to pass it to SymTable
static double MatchNumber()
{
  double value=0.0;
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
  if (minus) value=-value;
  return value;
}

//----------------------------------------------------------------------------
// Parsing Routines for Arithmetic Expressions
#include "mathparser.inl"

//----------------------------------------------------------------------------
// Parsing Routines for Statements
#include "stmtparser.inl"

//----------------------------------------------------------------------------
// Parsing Routines for INIT
#include "initparser.inl"

void Parse_VarList()
{
       // Add identifier if it doesn't exist, else error
       if (SymTable->Search("GLOBAL")->local->Search(tokenstring))
          SemanticError("Identifier already defined!");
        else SymTable->Search("GLOBAL")->local->Insert(tokenstring,type);
  Match(T_ID);
  if (token==T_COMMA)
    {
       Match(T_COMMA);
       Parse_VarList();
    }
}

void Parse_VarDecl()
{
  if (token!=T_RB)
    switch(token)
      {
        case T_REAL:
        case T_VECTOR: 
        case T_ENTITY_VECTOR:
        case T_INT:
                            // Prepare for assigning this type
                            type=token;
                       Match(token);
                       Parse_VarList();
                       Match(T_SEMICOLON);
                       Parse_VarDecl();
                       break;
      }
}

//----------------------------------------------------------------------------
// Parsing Routines for Entities
#include "entityparser.inl"

static int EntryPoint=-1;

void Parse_ScriptElements()
{
  if (token!=T_RB)
    switch(token)
      {
        case T_ENTITY: // A group of polygons!
                       Parse_Entity();
                       Parse_ScriptElements();
                       break;
        case T_VARS: // Variable Definition
                     Match(T_VARS);
                     Match(T_LB);
                     Parse_VarDecl();
                          SymTable->Search("GLOBAL")->SetMax(GetAddress(T_INT),
                                           GetAddress(T_REAL));
                     Match(T_RB);
                     Parse_ScriptElements();
                     break;
        case T_INIT: // Variable Initialization
                     Match(T_INIT);
                     Match(T_LB);
                     Parse_VarInit();
                     Match(T_RB);
                     Parse_ScriptElements();
                     break;
        case T_MAIN: // Main Function called each frame
                          EntryPoint=ICode->GetAddress()+1;
                     Match(T_MAIN);
                          // Breakpoint
                          START=true;
                     Parse_StmtBlock();
                     Parse_ScriptElements();
                          // Add Maximum Number of Entries in Symbol Table!
                          SymTable->Search("TEMPORAL")->SetMax(
                                           GetNextTemporal(T_INT).data,
                                           GetNextTemporal(T_REAL).data);
                          SymTable->Search("CONSTANT")->SetMax(ConstInt+1,ConstReal+1);
                     break;
      }
}

void BeginParse()
{
       // Perform Initialization of SymbolTable and Auxiliar Stacks
       SymTable=new ProcedureDirectory();
       SymTable->Insert("GLOBAL");
       SymTable->Insert("CONSTANT");
       // Used Constants 0,1,0.0,1.0
       SymTable->Search("CONSTANT")->local->Insert("0",T_NUM_INT);
       SymTable->Search("CONSTANT")->local->Insert("1",T_NUM_INT);
       SymTable->Search("CONSTANT")->local->Insert("1.0",T_NUM_REAL);
       SymTable->Search("CONSTANT")->local->Insert("0.0",T_NUM_REAL);
       SymTable->Insert("TEMPORAL");
       ResetZone();
       OperandStack=new TokenStack();
       OperStack=new TokenStack();
       JumpStack=new TokenStack();
       ICode=new QuadList();
   token=GetToken();
   Match(T_SCRIPT);
   Match(T_STRING);
   Match(T_LB);
   Parse_ScriptElements();
   Match(T_RB);
       // Add Last Quadruple, of end (a return to main)
       GEN_ICODE_LOOP_MAIN(EntryPoint);
       // Clear SymbolTable and Auxiliar Stacks
       ICode->Dump();
//       delete ICode; ERASED AT END OF Engine! USED BY VIRTUAL MACHINE
       delete JumpStack;
       delete OperStack;
       delete OperandStack;
//       delete SymTable; ERASED AT END OF Engine! USED BY VIRTUAL MACHINE
}

