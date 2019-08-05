// Aux Printing Routines
// (C) 1999 by A'r-Ci-O'oh

#include "script.h"
#include "tokens.h"
#include "scanner.h"
#include "print.h"

// Prints token data @ listfile
void PrintToken(TokenType token,const char *tokenstring)
{
  switch(token)
    {
       case T_SCRIPT:

       case T_VARS:
       case T_WORLD:
       case T_MAIN:

       case T_IF:
       case T_ELSE:
       case T_WHILE:

       case T_INT:
       case T_REAL:
                    fprintf(script_list,"T_xxxxx (Reserved Word): %s\n",tokenstring);
                    break;
       case T_STRING:
                    fprintf(script_list,"T_STRING: \"%s\"\n",tokenstring);
                    break;
       case T_ID:
                    fprintf(script_list,"T_ID Identifier: %s\n",tokenstring);
                    break;
       case T_NUM_INT:
                    fprintf(script_list,"T_NUM_INT Constant: %s\n",tokenstring);
                    break;
       case T_NUM_REAL:
                    fprintf(script_list,"T_NUM_REAL Constant: %s\n",tokenstring);
                    break;
       case T_ERROR:
                    fprintf(script_list,"T_ERROR Error: %s\n",tokenstring);
                    break;
       case T_EOF:
                    fprintf(script_list,"--- END OF FILE ---\n");
                    break;
       case T_ASSIGN:
                    fprintf(script_list,"T_ASSIGN: =\n");
                    break;
       case T_EQ:
                    fprintf(script_list,"T_EQ: ==\n");
                    break;
       case T_NOTEQ:
                    fprintf(script_list,"T_NOTEQ: !=\n");
                    break;
       case T_GTEQ:
                    fprintf(script_list,"T_GTEQ: >=\n");
                    break;
       case T_GT:
                    fprintf(script_list,"T_GT: >\n");
                    break;
       case T_LWEQ:
                    fprintf(script_list,"T_LWEQ: <=\n");
                    break;
       case T_LW:
                    fprintf(script_list,"T_LW: <\n");
                    break;
       case T_NOT:
                    fprintf(script_list,"T_NOT: !\n");
                    break;
       case T_COMMA:
                    fprintf(script_list,"T_COMMA: ,\n");
                    break;
       case T_SEMICOLON:
                    fprintf(script_list,"T_SEMICOLON: ;\n");
                    break;
       case T_COLON:
                    fprintf(script_list,"T_COLON: :\n");
                    break;
       case T_TIMES:
                    fprintf(script_list,"T_TIMES: *\n");
                    break;
       case T_OVER:
                    fprintf(script_list,"T_OVER: /\n");
                    break;
       case T_MOD:
                    fprintf(script_list,"T_MOD: %%\n");
                    break;
       case T_PLUS:
                    fprintf(script_list,"T_PLUS: +\n");
                    break;
       case T_MINUS:
                    fprintf(script_list,"T_MINUS: -\n");
                    break;
       case T_PLUSPLUS:
                    fprintf(script_list,"T_PLUSPLUS: ++\n");
                    break;
       case T_MINUSMINUS:
                    fprintf(script_list,"T_MINUSMINUS: --\n");
                    break;
       case T_LP:
                    fprintf(script_list,"T_LP: (\n");
                    break;
       case T_RP:
                    fprintf(script_list,"T_RP: )\n");
                    break;
       case T_LB:
                    fprintf(script_list,"T_LB: {\n");
                    break;
       case T_RB:
                    fprintf(script_list,"T_RB: }\n");
                    break;
    }
}

void SyntaxError(const char *msg)
{
  fprintf(script_list,"\n>>> ");
  fprintf(script_list,"Syntax Error @ Line %d: %s",lineno,msg);
}

void SemanticError(const char *msg)
{
  fprintf(script_list,"\n>>> ");
  fprintf(script_list,"Semantic Error @ Line %d: %s\n",lineno,msg);
}

void Report(const char *msg)
{
  fprintf(script_list,"\n    --> %s <--\n",msg);
}

void ReportEntry(const char *idname)
{
  fprintf(script_list,"       < Added Symbol %s >\n",idname);
}

