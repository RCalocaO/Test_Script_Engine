// ICode Generator
// (C) 1999 by A'r-Ci-O'oh

#include "../Scanner/script.h"
#include "scriptparser.h"
#include "proc_dir.h"
#include "address.h"
#include "stacks.h"
#include "quad.h"
#include "semantic.h"
#include "icode.h"

// Assign one vector to another
void GEN_ICODE_ASSIGN_VECTOR(TokenItem oper,TokenItem op1,TokenItem result)
{
  if ((op1.type==T_NUM_INT) || (op1.type==T_INT))
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
    }
     Quad q;
     ;
     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data);
     q.Set(2,Q_EMPTY,"<empty>",-1);
     q.Set(3,Q_ADDR,result.name,result.data);
     ICode->PasteQuad(q);

     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data+1);
     q.Set(2,Q_EMPTY,"<empty>",-1);
     q.Set(3,Q_ADDR,result.name,result.data+1);
     ICode->PasteQuad(q);

     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data+2);
     q.Set(2,Q_EMPTY,"<empty>",-1);
     q.Set(3,Q_ADDR,result.name,result.data+2);
     ICode->PasteQuad(q);
}

void GEN_ICODE_OPERAND_OP1_NONE_RESULTINSTACK()
{
  // Pop All Elements from Stacks
  TokenItem op1=OperandStack->Pop();
  TokenItem result=OperandStack->Pop();
  TokenItem oper=OperStack->Pop();

  if (((result.type==T_ENTITY_VECTOR) || (result.type==T_VECTOR)) &&
      ((op1.type==T_ENTITY_VECTOR) || (op1.type==T_VECTOR)) &&
      (oper.type=T_ASSIGN))
    {
      GEN_ICODE_ASSIGN_VECTOR(oper,op1,result);
      return;
    }

  SEMANTIC_VALIDATE_OP1_RESULT(op1,result,oper);

  // Generate Quadruple
     // --- > QUADRUPLE GENERATION CODE INSERT < ---
     Quad q;
     ;
     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data);
     q.Set(2,Q_EMPTY,"<empty>",-1);
     q.Set(3,Q_ADDR,result.name,result.data);
     ICode->PasteQuad(q);
  // And push to Operand Stack the Result
  OperandStack->Push(result.name,result.type,result.data);
}

// Generates code for operations with vectors
void GEN_ICODE_VECTOR_OPERATION(TokenItem oper,TokenItem op1,TokenItem op2,TokenItem result)
{
     Quad q;
     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data);
     q.Set(2,Q_ADDR,op2.name,op2.data);
     q.Set(3,Q_ADDR,result.name,result.data);
     ICode->PasteQuad(q);

     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data+1);
     q.Set(2,Q_ADDR,op2.name,op2.data+1);
     q.Set(3,Q_ADDR,result.name,result.data+1);
     ICode->PasteQuad(q);

     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data+2);
     q.Set(2,Q_ADDR,op2.name,op2.data+2);
     q.Set(3,Q_ADDR,result.name,result.data+2);
     ICode->PasteQuad(q);
}


// Generates the oper op1 op2 res quadruple and gets res from next avail
void GEN_ICODE_OPERAND_OP1_OP2_GETRESULT()
{
  // Pop All Elements from Stacks
  TokenItem op2=OperandStack->Pop();
  TokenItem op1=OperandStack->Pop();
  TokenItem oper=OperStack->Pop();
  // Result type according to semantic validations and promotions
  TokenType ResultType=
      SEMANTIC_VALIDATE_OP1_OP2_OPER(op1,op2,oper);

  // Get Next Temporal.
  TokenItem result=GetNextTemporal(ResultType);
  if ((ResultType==T_VECTOR) || (ResultType==T_ENTITY_VECTOR))
    {
       GEN_ICODE_VECTOR_OPERATION(oper,op1,op2,result);
       // And push to Operand Stack the Result
       OperandStack->Push(result.name,result.type,result.data);
       return;
    }
  // Generate Quadruple
     // --- > QUADRUPLE GENERATION CODE INSERT < ---
     Quad q;
     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data);
     q.Set(2,Q_ADDR,op2.name,op2.data);
     q.Set(3,Q_ADDR,result.name,result.data);
     ICode->PasteQuad(q);
  // And push to Operand Stack the Result
  OperandStack->Push(result.name,result.type,result.data);
}

// Performs and Generates code for all pending multiplicative operations
void GEN_ICODE_MULT()
{
  // First check if it is *,/,%,||
  switch(OperStack->Top()->type)
    {
        case T_TIMES:
        case T_OVER:
        //case T_OR:
        case T_MOD:
                     GEN_ICODE_OPERAND_OP1_OP2_GETRESULT();
                     break;
    }
}

// Performs and Generates code for all pending additive operations
void GEN_ICODE_ADD()
{
  // First check if it is +,-,&&
  switch(OperStack->Top()->type)
    {
        case T_PLUS:
        case T_MINUS:
        //case T_AND:
                     GEN_ICODE_OPERAND_OP1_OP2_GETRESULT();
                     break;
    }
}

// Part for If Testing Condition
void GEN_IF_TEST_CODE()
{
  // Result of Expr is in Operand Stack
  TokenItem condition=OperandStack->Pop();
  // Generate Quadruple
  Quad q;
     ;
  q.Set(0,Q_OPER,"JMPF",T_JMPF);
  q.Set(1,Q_ADDR,condition.name,condition.data);
  q.Set(2,Q_EMPTY,"<empty>",-1);
  q.Set(3,Q_LABEL,"LABEL",-1);        // Stays Waiting!
  ICode->PasteQuad(q);
  // Now retrieve current quad number
  int pending_jmpf=ICode->GetAddress();
  // And add to Waiting Jump Stack
  JumpStack->Push("<JMPF_LABEL>",T_NUM_INT,pending_jmpf);
}

// Assumes PC has just parsed last stmt in the IF, so PC->last stmt in IF
void GEN_IF_EXIT_CODE()
{
  TokenItem label=JumpStack->Pop();
  int actual=ICode->GetAddress();
  ICode->FillAddress(label.data,actual+1);
}

void GEN_ELSE_CODE()
{
  // Fill out the jump for the ELSE part
  TokenItem label=JumpStack->Pop();
  int actual=ICode->GetAddress();
  ICode->FillAddress(label.data,actual+2);
  // Generate Goto Quadruple
  Quad q;
     ;
  q.Set(0,Q_OPER,"JMP",T_JMP);
  q.Set(1,Q_EMPTY,"<empty>",-1);
  q.Set(2,Q_EMPTY,"<empty>",-1);
  q.Set(3,Q_LABEL,"LABEL",-1);        // Stays Waiting!
  ICode->PasteQuad(q);
  // Now retrieve current quad number
  int pending_jmp=ICode->GetAddress();
  // And add to Waiting Jump Stack
  JumpStack->Push("<JMP_LABEL>",T_NUM_INT,pending_jmp);
}


// Start While Code
void GEN_START_WHILE_CODE()
{
  // Add to Waiting Jump Stack
  int while_cond=ICode->GetAddress();
  JumpStack->Push("<WHILE_COND_LABEL>",T_NUM_INT,while_cond+1);
}

// Part for While Testing Condition
void GEN_WHILE_TEST_CODE()
{
  // Result of Expr is in Operand Stack
  TokenItem condition=OperandStack->Pop();
  // Generate Quadruple
  Quad q;
     ;
  q.Set(0,Q_OPER,"JMPF",T_JMPF);
  q.Set(1,Q_ADDR,condition.name,condition.data);
  q.Set(2,Q_EMPTY,"<empty>",-1);
  q.Set(3,Q_LABEL,"LABEL",-1);        // Stays Waiting!
  ICode->PasteQuad(q);
  // Now retrieve current quad number
  int while_stmt=ICode->GetAddress();
  // And add to Waiting Jump Stack
  JumpStack->Push("<WHILE_STMT_LABEL>",T_NUM_INT,while_stmt);
}

// Part for While Return
void GEN_WHILE_END_CODE()
{
  // Generate Quadruple for goto condition
  TokenItem out_while=JumpStack->Pop();
  TokenItem goto_cond=JumpStack->Pop();
  Quad q;
     ;
  q.Set(0,Q_OPER,"JMP",T_JMP);
  q.Set(1,Q_LABEL,"<empty>",-1);
  q.Set(2,Q_EMPTY,"<empty>",-1);
  q.Set(3,Q_ADDR,"WHILE_COND_LABEL",goto_cond.data);
  ICode->PasteQuad(q);
  // Fill out false exit code for while condition
  int actual=ICode->GetAddress();
  ICode->FillAddress(out_while.data,actual+1);
}

void GEN_START_REPEAT_CODE()
{
  // Add to Waiting Jump Stack
  int while_cond=ICode->GetAddress();
  JumpStack->Push("<REPEAT_START_LABEL>",T_NUM_INT,while_cond+1);
}

void GEN_REPEAT_END_CODE()
{
  // Result of Expr is in Operand Stack
  TokenItem condition=OperandStack->Pop();
  TokenItem repeat_again=JumpStack->Pop();

  // Generate Quadruple
  Quad q;
  q.Set(0,Q_OPER,"JMPF",T_JMPF);
  q.Set(1,Q_ADDR,condition.name,condition.data);
  q.Set(2,Q_EMPTY,"<empty>",-1);
  q.Set(3,Q_LABEL,"LABEL",repeat_again.data);
  ICode->PasteQuad(q);
}

// which is either PLUSPLUS or MINUSMINUS
// What we do is just create a quad like (oper dest 1 dest)
void GEN_UNARY(TokenType which)
{
  // Pop Operand Stack and push it to get a local copy
  TokenItem dest=OperandStack->Pop();
  OperandStack->Push(dest.name,dest.type,dest.data);

  char one[3];
  TokenType type;

  if (dest.type==T_INT)
    {
      strcpy(one,"1");
      type=T_NUM_INT;
    }
   else
     {
       strcpy(one,"1.0");
       type=T_NUM_REAL;
     }
  // Insert Operation
  if (which==T_PLUSPLUS)
    OperStack->Push("+",T_PLUS,T_PLUS);
   else
     OperStack->Push("-",T_MINUS,T_MINUS);
  // Insert the "1"
  LocalVariableTable *where=SymTable->Search("CONSTANT")->local;
  LV_Entry *constnt=where->Search(one,false);
  // If it is not found...
  if (constnt==NULL)
     {
       // Insert it
       where->Insert(one,type);
       //ReportEntry(one);
       constnt=where->Search(one,false);
       if (type=T_NUM_INT)
         if (ConstInt<constnt->address)
           ConstInt=constnt->address;
       if (type=T_NUM_REAL)
         if (ConstReal<constnt->address)
           ConstReal=constnt->address;
     }
  // Push to Operand Stack (I'm only interested in address!)
  OperandStack->Push(constnt->name,constnt->type,constnt->address);
  // Push dest=source
  OperandStack->Push(dest.name,dest.type,dest.data);
  // Generate Quadruple:

  // Pop All Elements from Stacks
  TokenItem result=OperandStack->Pop();
  TokenItem op2=OperandStack->Pop();
  TokenItem op1=OperandStack->Pop();
  TokenItem oper=OperStack->Pop();
  // Generate Quadruple
     // --- > QUADRUPLE GENERATION CODE INSERT < ---
     Quad q;
     ;
     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data);
     q.Set(2,Q_ADDR,op2.name,op2.data);
     q.Set(3,Q_ADDR,result.name,result.data);
     ICode->PasteQuad(q);
  // And push to Operand Stack the Result
  OperandStack->Push(result.name,result.type,result.data);
}

void GEN_ICODE_LOOP_MAIN(int EntryPoint)
{
     // --- > QUADRUPLE GENERATION CODE INSERT < ---
     Quad q;
     q.Set(0,Q_OPER,"JMP",T_JMP);
     q.Set(1,Q_EMPTY,"<empty>",-1);
     q.Set(2,Q_EMPTY,"<empty>",-1);
     q.Set(3,Q_LABEL,"LABEL",EntryPoint);
     ICode->PasteQuad(q);
}

void GEN_PRINT_CODE()
{
  // Pop Element from Stacks
  TokenItem op1=OperandStack->Pop();

  // Generate Quadruple
     // --- > QUADRUPLE GENERATION CODE INSERT < ---
     Quad q;
     ;
     q.Set(0,Q_OPER,"PRINT",T_PRINT);
     q.Set(1,Q_ADDR,op1.name,op1.data);
     q.Set(2,Q_EMPTY,"<empty>",-1);
     q.Set(3,Q_EMPTY,"<empty>",-1);
     ICode->PasteQuad(q);
  if ((op1.type==T_VECTOR) || (op1.type==T_ENTITY_VECTOR))
    {
     q.Set(0,Q_OPER,"PRINT",T_PRINT);
     q.Set(1,Q_ADDR,op1.name,op1.data+1);
     q.Set(2,Q_EMPTY,"<empty>",-1);
     q.Set(3,Q_EMPTY,"<empty>",-1);
     ICode->PasteQuad(q);

     q.Set(0,Q_OPER,"PRINT",T_PRINT);
     q.Set(1,Q_ADDR,op1.name,op1.data+2);
     q.Set(2,Q_EMPTY,"<empty>",-1);
     q.Set(3,Q_EMPTY,"<empty>",-1);
     ICode->PasteQuad(q);
    }
}

// Negates a value. Quadruple - 0 value value
void GEN_ICODE_NEGATE()
{
  // Pop Operand Stack and push it to get a local copy
  TokenItem dest=OperandStack->Pop();
  OperandStack->Push(dest.name,dest.type,dest.data);
  bool vector=false;
  if ((dest.type==T_VECTOR) || (dest.type==T_ENTITY_VECTOR))
    vector=true;

  char zero[4] = "0.0";
  TokenType type;

  if ((dest.type==T_INT) || (dest.type==T_NUM_INT))
    {
      strcpy(zero,"0");
      type=T_NUM_INT;
    }
   else
     {
       strcpy(zero,"0.0");
       type=T_NUM_REAL;
     }
  // Insert Operation
  OperStack->Push("-",T_MINUS,T_MINUS);
  // Insert the "0"
  LocalVariableTable *where=SymTable->Search("CONSTANT")->local;
  LV_Entry *constnt=where->Search(zero,false);
  // If it is not found...
  if (constnt==NULL)
     {
       // Insert it
       where->Insert(zero,type);
       //ReportEntry(one);
       constnt=where->Search(zero,false);
     }
  // Push to Operand Stack (I'm only interested in address!) Constant ZERO!
  OperandStack->Push(constnt->name,constnt->type,constnt->address);
  // Push dest=source
//  OperandStack->Push(dest.name,dest.type,dest.data);

  // Generate Quadruple:
  // Pop All Elements from Stacks
  TokenItem result=GetNextTemporal(dest.type);
  TokenItem op1=OperandStack->Pop();
  TokenItem op2=OperandStack->Pop();
  TokenItem oper=OperStack->Pop();

  // Generate Quadruple
     // --- > QUADRUPLE GENERATION CODE INSERT < ---
     Quad q;
     q.Set(0,Q_OPER,oper.name,oper.data);
     q.Set(1,Q_ADDR,op1.name,op1.data);
     q.Set(2,Q_ADDR,op2.name,op2.data);
     q.Set(3,Q_ADDR,result.name,result.data);
     ICode->PasteQuad(q);
     if (vector)
      {
        q.Set(0,Q_OPER,oper.name,oper.data);
        q.Set(1,Q_ADDR,op1.name,op1.data);
        q.Set(2,Q_ADDR,op2.name,op2.data+1);
        q.Set(3,Q_ADDR,result.name,result.data+1);
        ICode->PasteQuad(q);

        q.Set(0,Q_OPER,oper.name,oper.data);
        q.Set(1,Q_ADDR,op1.name,op1.data);
        q.Set(2,Q_ADDR,op2.name,op2.data+1);
        q.Set(3,Q_ADDR,result.name,result.data+1);
        ICode->PasteQuad(q);
      }
  // And push to Operand Stack the Result
  OperandStack->Push(result.name,result.type,result.data);
}


