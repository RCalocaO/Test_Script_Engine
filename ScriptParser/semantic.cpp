// Semantics
// (C) 1999 by A'r-Ci-O'oh

#include "../Scanner/script.h"
#include "semantic.h"
#include "address.h"
#include "quad.h"


// Validation that equal types cause a condition result of INT
TokenType SEMANTIC_VALIDATE_CONDITION(TokenItem& result,TokenItem& oper)
{
    switch(oper.type)
      {
         case T_LW:
         case T_LWEQ:
         case T_GT:
         case T_GTEQ:
         case T_EQ:
         case T_NOTEQ:
                  return T_INT;
         default: return result.type;

      }
}

// Validation for int and real. If not equal, promotes!
TokenType SEMANTIC_VALIDATE_OP1_OP2_OPER(TokenItem& a,
          TokenItem& b,TokenItem& oper)
{
  // If same type, if not condition return type else return int
  if (a.type==b.type)
    return SEMANTIC_VALIDATE_CONDITION(a,oper);

  // if both are int or real or vector, return that same!
  if (
     ((a.type==T_NUM_INT) || (a.type==T_INT))
     &&
     ((T_INT==b.type) || (T_NUM_INT==b.type))
     )
    return T_INT;
  if (
     ((a.type==T_NUM_REAL) || (a.type==T_REAL))
     &&
     ((T_REAL==b.type) || (T_NUM_REAL==b.type))
     )
    return T_REAL;
  if (
     ((a.type==T_VECTOR) || (a.type==T_ENTITY_VECTOR))
     &&
     ((T_ENTITY_VECTOR==b.type) || (T_VECTOR==b.type))
     )
    return T_VECTOR;
/*
  if ((a.type==T_INT) && (T_NUM_INT==b.type)) return T_INT;
  if ((a.type==T_NUM_REAL) && (T_REAL==b.type)) return T_REAL;
*/

  if ((a.type==T_REAL) && (T_NUM_REAL==b.type)) return T_REAL;

  if (((a.type==T_NUM_INT) || (a.type==T_INT)) &&
      ((b.type==T_NUM_REAL) || (b.type==T_REAL)))
    {
      // Need to make a new quadruple for promoting a to real
      TokenItem promoted=GetNextTemporal(T_REAL);
      Quad q;
      q.Set(0,Q_OPER,"INT_2_REAL",T_INT2REAL);
      q.Set(1,Q_ADDR,a.name,a.data);
      q.Set(2,Q_EMPTY,"<empty>",-1);
      q.Set(3,Q_ADDR,promoted.name,promoted.data);
      ICode->PasteQuad(q);

      // Get a new temp from avail and return it in a
      a=promoted;
      return SEMANTIC_VALIDATE_CONDITION(a,oper);
    }
   else if (((b.type==T_NUM_INT) || (b.type==T_INT)) &&
      ((a.type==T_NUM_REAL) || (a.type==T_REAL)))
    {
      // Need to make a new quadruple for promoting b to real
      TokenItem promoted=GetNextTemporal(T_REAL);
      Quad q;
      q.Set(0,Q_OPER,"INT_2_REAL",T_INT2REAL);
      q.Set(1,Q_ADDR,b.name,b.data);
      q.Set(2,Q_EMPTY,"<empty>",-1);
      q.Set(3,Q_ADDR,promoted.name,promoted.data);
      ICode->PasteQuad(q);

      // Get a new temp from avail and return it in a
      b=promoted;
      return SEMANTIC_VALIDATE_CONDITION(a,oper);
    }
}

// Validates that result has type of op1
void SEMANTIC_VALIDATE_OP1_RESULT(TokenItem& op1,TokenItem& result,
     TokenItem& oper)
{
  if (((result.type==T_VECTOR) || (result.type==T_ENTITY_VECTOR)) &&
      ((op1.type==T_NUM_INT) || (op1.type==T_INT)))
        {
          // Need to make a new quadruple for promoting a to real
          TokenItem promoted=GetNextTemporal(T_REAL);
          Quad q;
          q.Set(0,Q_OPER,"INT_2_REAL",T_INT2REAL);
          q.Set(1,Q_ADDR,op1.name,op1.data);
          q.Set(2,Q_EMPTY,"<empty>",-1);
          q.Set(3,Q_ADDR,promoted.name,promoted.data);
          ICode->PasteQuad(q);
          // Return new temp from avail
          op1=promoted;
          return;
        }
  if (((op1.type==T_NUM_INT) || (op1.type==T_INT)) &&
      ((result.type==T_NUM_REAL) || (result.type==T_REAL)))
    {
      // Need to make a new quadruple for promoting a to real
      TokenItem promoted=GetNextTemporal(T_REAL);
      Quad q;
      q.Set(0,Q_OPER,"INT_2_REAL",T_INT2REAL);
      q.Set(1,Q_ADDR,op1.name,op1.data);
      q.Set(2,Q_EMPTY,"<empty>",-1);
      q.Set(3,Q_ADDR,promoted.name,promoted.data);
      ICode->PasteQuad(q);

      // Get a new temp from avail and return it in a
      op1=promoted;
      return ;
    }
   else if (((result.type==T_NUM_INT) || (result.type==T_INT)) &&
      ((op1.type==T_NUM_REAL) || (op1.type==T_REAL)))
    {
      // Need to make a new quadruple for promoting a to real
      TokenItem promoted=GetNextTemporal(T_INT);
      Quad q;
      q.Set(0,Q_OPER,"REAL_2_INT",T_REAL2INT);
      q.Set(1,Q_ADDR,op1.name,op1.data);
      q.Set(2,Q_EMPTY,"<empty>",-1);
      q.Set(3,Q_ADDR,promoted.name,promoted.data);
      ICode->PasteQuad(q);

      // Get a new temp from avail and return it in a
      op1=promoted;
      return ;
    }
}

