// Aux Print Routines
// (C) 1999 by A'r-Ci-O'oh

#ifndef __PRINT_H__
#define __PRINT_H__

#include "script.h"

// Displays which T_xxxx is token and its tokenstring
extern void PrintToken(TokenType token,const char *tokenstring);

// This is used only when Tracing the Parsing process!
typedef enum
{
  // Main States
  P_SCRIPT,P_VAR,P_BACK,P_NF,
  P_NF_STMT,P_VAR_STMT,P_BACK_STMT,
  // Variable Declaration
  P_INT_STMT,P_REAL_STMT,P_VECTOR2_STMT,P_POLY2_STMT,
  P_POLY2_SINGLE,P_VECTOR2_SINGLE,P_ID_SINGLE,P_ID_INITIALVALUE,
  P_1PARAM,P_2PARAM,P_3PARAM,P_4PARAM,P_5PARAM,P_EXPRESION
} ParseStateType;

// Displays what are we parsing currently
extern void PARSED(ParseStateType state);
extern void SyntaxError(const char *msg);
extern void SemanticError(const char *msg);
extern void Report(const char *msg);
extern void ReportEntry(const char *idname);

#endif